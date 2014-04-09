

-- CREATE LANGUAGE plpgsql;



--CREATE OR REPLACE FUNCTION testfunc(integer) RETURNS integer AS $PROC$
--          ....
--$PROC$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION InsertNewRun(CfgID INT)  RETURNS integer AS $$
DECLARE
    lastrun mainrun.RunNumber%TYPE;
BEGIN
     SELECT INTO lastrun GetLastRun();
   
     INSERT INTO MainRun(RunNumber,SubRunNumber,ConfigID,RunType,TimeStart) VALUES(lastrun+1,1,CfgID,333, 'now');


    RETURN lastrun+1;

END;
$$ LANGUAGE plpgsql;



------------------------------------------------------------------------------
-------------------------------------------------------------------------------
---- determine what the relation between ConfitType and ConfigID is.
--------------------------------------------------------------------------------
CREATE OR REPLACE FUNCTION InsertNewSubRun(CfgID INT,run INT DEFAULT -1) RETURNS integer AS $$
DECLARE
    lastrun mainrun.RunNumber%TYPE;
    lastsubrun mainrun.SubRunNumber%TYPE;
BEGIN
     IF run = -1  THEN
	  SELECT INTO lastrun GetLastRun();
     ELSE 
          lastrun := run;
     END IF;    

     SELECT INTO lastsubrun GetLastSubRun(lastrun);

     INSERT INTO MainRun(RunNumber,SubRunNumber,ConfigID,RunType,TimeStart) VALUES(lastrun,lastsubrun+1,CfgID,333,'now');


    RETURN lastsubrun+1;
END;
$$ LANGUAGE plpgsql;



-----------------------------------------------------------------------------
-------------------------------------------------------------------------------
CREATE OR REPLACE FUNCTION GetLastRun() RETURNS integer AS $$
DECLARE
    lastrun mainrun.RunNumber%TYPE;
BEGIN
    
    SELECT INTO lastrun RunNumber FROM MainRun  ORDER BY RunNumber DESC LIMIT 1;

    RETURN lastrun;
END;
$$ LANGUAGE plpgsql;


-----------------------------------------------------------------------------
-------------------------------------------------------------------------------
CREATE OR REPLACE FUNCTION GetLastSubRun(run INT) RETURNS integer AS $$
DECLARE
    lastsubrun mainrun.SubRunNumber%TYPE;
BEGIN
    
    SELECT INTO lastsubrun SubRunNumber FROM MainRun WHERE RunNumber=run ORDER BY SubRunNumber DESC LIMIT 1;

    RETURN lastsubrun;
END;
$$ LANGUAGE plpgsql;


-------------------------------------------------------------------------------
--------------------------------------------------------------------------------


--to work on pmtid for the time being:

CREATE OR REPLACE FUNCTION emp_stamp() RETURNS trigger AS $emp_stamp$
    BEGIN
        -- Check that empname and salary are given
        IF NEW.crate IS NULL THEN
            RAISE EXCEPTION 'crate cannot be null';
        END IF;
    --    IF NEW.parameters IS NULL THEN
    --        RAISE EXCEPTION '% cannot have parameters =0', NEW.crate;
      --  END IF;

       
     --   IF NEW.parameters < 0 THEN
     --       RAISE EXCEPTION '% cannot have a parameterx < 0', NEW.crate;
     --   END IF;

        -- Remember who changed the payroll when
        NEW.timestamp := 'now';
        NEW.userid := current_user;
        RETURN NEW;
    END;
$emp_stamp$ LANGUAGE plpgsql;

--CREATE TRIGGER emp_stamp BEFORE INSERT OR UPDATE ON pmtconfig
--    FOR EACH ROW EXECUTE PROCEDURE emp_stamp();



-- at least one line with the configuration must exist
CREATE OR REPLACE FUNCTION mainruncheck() RETURNS trigger AS $mainruncheck$
    DECLARE
    myrec RECORD;
    BEGIN
        -- Check that empname and salary are given
       SELECT INTO myrec * FROM mainconfigtable WHERE mainconfigtable.configid=NEW.configid;
       IF NOT FOUND THEN
	  RAISE EXCEPTION 'Configuration % not found', NEW.configid;
      END IF;

       
       -- Remember who changed the payroll when
      --  NEW.timestart := 'now';
       -- NEW.userid := current_user;
        RETURN NEW;
    END;
$mainruncheck$ LANGUAGE plpgsql;

--CREATE TRIGGER mainruncheck BEFORE INSERT OR UPDATE ON mainrun
 --   FOR EACH ROW EXECUTE PROCEDURE mainruncheck();




-- at least one line with the configuration must exist
CREATE OR REPLACE FUNCTION mainconfigcheck() RETURNS trigger AS $mainconfigcheck$
    DECLARE
    myrec RECORD;
    tablename ConfigLookUp.SubConfigName%TYPE;
    query   text;
    BEGIN
        -- Check that empname and salary are given
      RAISE INFO 'subconfigid % ', NEW.subconfigtype;

      SELECT  configname FROM configtypes WHERE configtypes.configtype=NEW.subconfigtype INTO tablename;

      RAISE INFO 'SELECTED % ', tablename;

     
   
  --  
    query := 'SELECT configid FROM ' || tablename || ' WHERE ' || tablename || '.configid =' || NEW.subconfigid ;

    RAISE INFO ' % ', query ;

    EXECUTE query INTO myrec; 
      IF myrec.configid IS NULL  THEN
	  RAISE EXCEPTION 'Configuration % not found', NEW.subconfigid;
      END IF;

     query := 'SELECT configid FROM ' || tablename || ' WHERE ' || tablename || '.configid =' || NEW.subconfigparentid ;

      RAISE INFO ' % ', query ;

      EXECUTE query INTO myrec; 
	IF myrec.configid IS NULL  THEN
	  RAISE EXCEPTION 'Parent Configuration % not found', NEW.subconfigparentid;
	END IF;
--       EXECUTE 'SELECT configid FROM '
-- 	|| tablename 
-- 	|| ' WHERE '
-- 	|| tablename
-- 	|| '.configid ='
-- 	|| substring(NEW.subconfigid,1,1) 
--       INTO myrec;
      --SELECT INTO myrec * FROM mainconfigtable WHERE mainconfigtable.configid=NEW.configid;
       


       
       -- Remember who changed the payroll when
       -- NEW.timestop := 'now';
       -- NEW.userid := current_user;
        RETURN NEW;
    END;
$mainconfigcheck$ LANGUAGE plpgsql VOLATILE STRICT;

--CREATE TRIGGER mainconfigcheck BEFORE INSERT OR UPDATE ON mainconfigtable
--    FOR EACH ROW EXECUTE PROCEDURE mainconfigcheck();

--------------------------------------------------------------------------------
-----------------------------------------------------------------------------------




-- at least one line with the configuration must exist
CREATE OR REPLACE FUNCTION ReturnConfigurationByNumber(requestedConfigid INT) RETURNS SETOF subconfigreturntype AS $$
    DECLARE
    mainconfrec RECORD;
    configlookuprec RECORD;
    subconfigconfigrec subconfigreturntype;
    subconfigquery  text ;
    BEGIN
       
	--select subconfigTypes and their numbers from the MainConfigTable

    FOR mainconfrec IN SELECT SubConfigType,SubConfigID,SubConfigParentID,SubConfigOnMask FROM MainConfigTable 
    WHERE Configid=requestedConfigid LOOP

    RAISE INFO ' subconfig Type: %, ID: %, SubConfigParentID: %,  ',mainconfrec.SubConfigType,mainconfrec.SubConfigID,mainconfrec.SubConfigParentID;
 
    -- querying the lookup table to find out, what it is that we're picking up from the subconfig table
    SELECT INTO configlookuprec * FROM ConfigLookUp WHERE ConfigLookup.SubConfigType=mainconfrec.SubConfigType;

    RAISE INFO ' config name: % and parameters: % ',configlookuprec.SubConfigName,configlookuprec.SetupColumns;

    -- Create JOIN of Main config table and subtable and return that
    subconfigquery := 'SELECT *  FROM ' || configlookuprec.SubConfigName || ' WHERE ' || configlookuprec.SubConfigName || 
	  '.ConfigID = '||mainconfrec.SubConfigID;

    RAISE INFO 'sub conf query: %',subconfigquery;

 
    for subconfigconfigrec in EXECUTE subconfigquery loop
       subconfigconfigrec.ConfigName:=configlookuprec.SubConfigName;  --inserting the name for faster lookup later
       subconfigconfigrec.ConfigType:=mainconfrec.SubConfigType;      --inserting type for faster lookup

      return next subconfigconfigrec;
    end loop;

  END LOOP;

  RETURN;
   
    END;
$$ LANGUAGE plpgsql VOLATILE STRICT;






--------------------------------------------------------------------------------
-----------------------------------------------------------------------------------
------------------------------------------------------------------------------------------

CREATE OR REPLACE FUNCTION CreateConfigurationType(configtabletype text,columns HSTORE) RETURNS INT AS $$
    DECLARE
    myrec RECORD;
    maskconfig INT;
    newconfig INT;
    query text;  
    BEGIN
        
    -- First find if a config like this exists, if not, insert it.
    IF EXISTS ( SELECT TRUE FROM ConfigLookUp WHERE SubConfigName = configtabletype )    
      THEN RAISE EXCEPTION '++++++++++ Configuration Type % Already Exists +++++++++++', configtabletype;
    END IF;

    SELECT INTO myrec * FROM  ConfigLookUp ORDER BY SubConfigType DESC;
    --SELECT INTO myrec * FROM ConfigLookUp WHERE ConfigName=configtabletype ORDER BY SubConfigType DESC;
    

    IF NOT FOUND THEN
	  newconfig:=1;
    ELSE 
          newconfig:=myrec.SubConfigType+1;
    END IF;


  -- INSERT test of HSTORE here.
  -- nchannels must exist and parameters must be ok. 
  --  etc... TBD


  --CREATE TABLE - they all look the same.

   query := 'CREATE TABLE '|| configtabletype ||' (
   ConfigID  INT NOT NULL   DEFAULT NULL,
   ParentConfigID  INT NOT NULL  DEFAULT -1,
   Crate  SMALLINT  NOT NULL DEFAULT 0,
   Channel  INT  NOT NULL DEFAULT 0,
   Parameters  HSTORE  NOT NULL DEFAULT ''nchannels=>0 '' ,
   userID  VARCHAR NULL DEFAULT NULL,
   TimeStamp  TIMESTAMP NULL DEFAULT NULL,
    PRIMARY KEY ( ConfigID,Crate,Channel ))';   

 
   EXECUTE query; 
-- now that we've created the table, let's also insert the name into the lookup table.
       

   query:= 'INSERT INTO ConfigLookUp(SubConfigType,SubConfigName,SetupColumns) VALUES('||newconfig||','''||configtabletype||''','''||columns||''')';
   RAISE INFO '%',query;
   execute query;
   RAISE INFO 'Inserted new configuration type with ID %',newconfig;
 
  
    RETURN newconfig;
    --END IF;
   
    END;
$$ LANGUAGE plpgsql VOLATILE STRICT;


------------------------------------------------------------------------

------------------------------------------------------------------------------------------

----- Syntax: SELECT InsertConfigurationSet('pmtconfig',1,3,5,'"nchannels"=>"4000","gain"=>"300"');

CREATE OR REPLACE FUNCTION GETQUERY2(configtabletype text,insubconfid INT,incrate INT, inchannel INT,columns HSTORE) RETURNS TEXT AS $$
DECLARE
  query text;
  BEGIN
  
  query := format('SELECT * FROM %s WHERE ConfigID=%s AND channel=%s AND crate=%s',configtabletype,insubconfid,inchannel,incrate);
--  return query;
--  SELECT query;
  EXECUTE query;
--  RETURN query;
  END;
$$ LANGUAGE plpgsql VOLATILE STRICT;



------------------------------------------------------------------------




CREATE OR REPLACE FUNCTION InsertConfigurationSet(configtabletype text,insubconfid INT,incrate INT, inchannel INT,columns HSTORE) RETURNS INT AS $$
    DECLARE
    myrec RECORD;
   -- maskconfig INT;
    newconfig INT;
    query text;  
    BEGIN
        
 -- First find if this configuration type exists. If not, don't do anything
    IF NOT EXISTS ( SELECT TRUE FROM ConfigLookUp WHERE SubConfigName = configtabletype )    
      THEN RAISE EXCEPTION '++++++++++ Configuration % is not defined yet! +++++++++++', configtabletype;
    END IF;

 -- Second find if a config like this exists. If does, don't do anything
    query := format('SELECT TRUE FROM %s WHERE ConfigID=%s AND channel=%s AND crate=%s',configtabletype,insubconfid,inchannel,incrate);
    EXECUTE query INTO myrec;
    IF NOT myrec.bool IS NULL THEN
      RAISE EXCEPTION '++++++++++ Configuration nr % Already Exists for Crate % and Channel % ++++++++++', insubconfid,incrate,inchannel;
    END IF;

 -- INSERT HSTORE checking, i.e. for each(hstore from ConfigLookUp Table, check that hstore key exists). TBD.
   IF NOT EXISTS (SELECT TRUE FROM ConfigLookUp WHERE SubConfigName = configtabletype AND columns ?& AKEYS(SetUpColumns))
     THEN RAISE EXCEPTION '++++++++++ Provided HSTORE columns are lacking necessary pieces! +++++++++++';
   END IF;

   query := 'INSERT INTO '||configtabletype||'(ConfigID,crate,channel,Parameters) VALUES('||insubconfid||','||incrate||','||inchannel||','''||columns||''')';
  -- RAISE INFO 'Inserted new configuration type with ID %',newconfig;
 
   execute query;
   -- RETURN newconfig;
    --END IF;
   RETURN 0;
    END;
$$ LANGUAGE plpgsql VOLATILE STRICT;


------------------------------------------------------------------------

DROP FUNCTION IF EXISTS InsertMainConfiguration( text, HSTORE) ;


CREATE OR REPLACE FUNCTION InsertMainConfiguration(subconfigparameters HSTORE,confname text DEFAULT 'no_name') RETURNS INT AS $$
    DECLARE
    myrec1 RECORD;
    myrec2 RECORD;
   -- maskconfig INT;
    newconfig INT;
    query text;  
    SubConfT INT;
    mainconfigexists INT;
    BEGIN
        

 --first loop on HSTORE keys and make sure that the subconfigurations selected exist: 
      -- First check if the provided Config ID already exsits in the MainConfigTable or not
    IF  (confname <> 'no_name' ) AND EXISTS (SELECT mainconfID FROM MainConfigTable WHERE MainConfigTable.ConfigName = confname)
        THEN RAISE EXCEPTION '+++++++++++++ Config with name % already exists in MainConfigTable, with ID: %! +++++++++++++',confname,mainconfID;
	RETURN -1;
    END IF;
    
    -- now we know that the name doesn't exist or the user doesn't care.
  
    mainconfigexists:=1;  -- assuming it exists. Let's be proven otherwise.
    --let's check if the configuration exists:
    for myrec1 IN SELECT (each(columns)).*
	LOOP
        --get subconfig type
        ------------------------------ this repeats the functionality from later.
            SELECT  SubconfigType FROM ConfigLookUp WHERE SubConfigName = myrec1.key INTO SubConfT;
	    IF NOT EXISTS( SELECT TRUE FROM MainconfigTable WHERE SubConfT = myrec1.key AND subconfigid=myrec1.val )
	      THEN mainconfigexists=0;
	      EXIT;   -- break out of loop. We already know, we need a new config.
	    END IF;
	END LOOP;  -- end checking if mainconfig exists loop.

     ---------- !!!!! warning! currently there is no check to make sure all of the above fall into the same mainconfigid!
     ------------!!!!!!!!!!

    IF mainconfigexists = 1
      THEN RAISE EXCEPTION '+++++++++++++ Config already exists in MainConfigTable, with ID: %! +++++++++++++',-1;
    END IF;

    ----------- To be improved

    -- Next check if specified SubConfigType and SubConfigID are available or not
    for myrec1 IN SELECT (each(columns)).*
    LOOP
        -- Check if SubConfigType is valid or not
        RAISE NOTICE 'key:%, value: %', myrec1.key, myrec1.value;
	IF NOT EXISTS ( SELECT TRUE FROM ConfigLookUp WHERE SubConfigName = myrec1.key)
	    THEN RAISE EXCEPTION '++++++++++++ Configuration % is not defined! +++++++++++++++', myrec1.key;
	    RETURN 1;
	END IF;
	-- Check if SubConfigID is valid or not
	query := format('SELECT TRUE FROM %s WHERE COnfigID=%s',myrec1.key,myrec1.value);
	EXECUTE query INTO myrec2;
	IF myrec2.bool IS NULL
	    THEN RAISE EXCEPTION '++++++++++++ Configuration % does not contain ConfigID=%++++++++++++',myrec1.key,myrec1.value;
	    RETURN 1;
	END IF;
	
    END LOOP;
    
    --------------------------------- Find the last entry
    SELECT configID FROM MainConfigTable ORDER BY configID ASC LIMIT 1 INTO newconfig ;
  
   
    -- Reaching this point means input values are valid. Let's insert.
   for myrec1 IN SELECT (each(columns)).*
    LOOP
    SELECT SubconfigType FROM ConfigLookUp WHERE SubConfigName = myrec1.key INTO SubConfT;
    INSERT INTO MainconfigTable (ConfigID,
				  SubConfigType,
				  SubConfigID, 
				  SubConfigOnMask,
				  ConfigName) VALUES(newconfig+1,SubConfT,myrec1.val,65636,confname);

    END LOOP;

   RETURN newconfig+1;
   END;
$$ LANGUAGE plpgsql VOLATILE STRICT;


------------------------------------------------------------------------







DROP FUNCTION IF EXISTS GetSubConfig( text, INT) ;

------------------create TYPES for each table?



-- at least one line with the configuration must exist
CREATE OR REPLACE FUNCTION GetSubConfig(tablename text,mainconfigid INT) RETURNS SETOF subconfigreturntype AS $$
    DECLARE
    myrec RECORD;
    conftype ConfigLookUp.SubConfigType%TYPE;
    mainconfrow mainconfigtable%ROWTYPE; --.subconfigparentid%TYPE;
    subconfigconfigrec subconfigreturntype;
    query   text;
    testquery text;
    mainconfquery text;
    BEGIN
        -- Check that empname and salary are given
      RAISE INFO 'subconfigid % ', tablename;

       --'SELECT configid FROM ' || tablename || ' WHERE ' || tablename || '.configid =' || subconfigid ;
      testquery :='SELECT  SubConfigType FROM ConfigLookup WHERE ConfigLookup.SubConfigName=' || quote_literal(tablename);
      RAISE INFO 'testquery % ', testquery;
      EXECUTE testquery INTO conftype;

      RAISE INFO 'SELECTED % ', conftype;

      mainconfquery :='SELECT  * FROM mainconfigtable WHERE subconfigtype=' || conftype || 'AND configid='||mainconfigid;  

      RAISE INFO ' % ', mainconfquery ;
   
      EXECUTE mainconfquery INTO mainconfrow;

      RAISE INFO 'conf: %  , subconf: %   ,  parent: %  ', mainconfrow.configid, mainconfrow.subconfigid ,  mainconfrow.subconfigparentid ;
  
  ---- SELECT   
      query := 'SELECT * FROM ' || tablename || ' WHERE ' || tablename || '.configid =' || mainconfrow.subconfigparentid ;

      for subconfigconfigrec in EXECUTE query loop
      subconfigconfigrec.ConfigName:=tablename;  --inserting the name for faster lookup later
      subconfigconfigrec.ConfigType:=conftype;      --inserting type for faster lookup

      return next subconfigconfigrec;
      end loop;
      return;

     -- RAISE INFO ' % ', query ;

     -- EXECUTE query INTO myrec; 
     -- IF myrec.parameterx IS NULL  THEN
	--  RAISE EXCEPTION 'Configuration % not found', subconfigid;
     -- END IF;

--- SELECT PARENT, THEN SELECT INTO RECORD the STANDARD CONFIG WHERE CRATE and CHANNEL
--- DO we want to get the ultimate Parent?
--- This means that the flattened parent needs to have a -1? 
--- need to keep a list of previous parents when looping



--      query := 'SELECT configid FROM ' || tablename || ' WHERE ' || tablename || '.configid =' || NEW.subconfigparentid ;
-- 
    --   RAISE INFO 'result? % ', myrec ;
-- 
--       EXECUTE query INTO myrec; 
-- 	IF myrec.configid IS NULL  THEN
-- 	  RAISE EXCEPTION 'Parent Configuration % not found', NEW.subconfigparentid;
-- 	END IF;
       


       
       -- Remember who changed the payroll when
       -- NEW.timestop := 'now';
       -- NEW.userid := current_user;
       -- RETURN myrec;
    END;
$$ LANGUAGE plpgsql VOLATILE STRICT;




















