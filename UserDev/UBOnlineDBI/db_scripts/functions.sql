

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

DROP FUNCTION IF EXISTS ExistSubConfig(SCfgName TEXT);
CREATE OR REPLACE FUNCTION ExistSubConfig(SCfgName TEXT) RETURNS INT AS $$
   SELECT 1 FROM ConfigLookUp WHERE SubConfigName = SCfgName LIMIT 1
$$ LANGUAGE SQL;

DROP FUNCTION IF EXISTS ListSubConfigParameters(SCfgName TEXT);
CREATE OR REPLACE FUNCTION ListSubConfigParameters(SCfgName TEXT) RETURNS SETOF TEXT AS $$
   SELECT skeys(setupcolumns) FROM configlookup WHERE subconfigname = SCfgName
$$ LANGUAGE SQL;

DROP FUNCTION IF EXISTS ListSubConfigNameAndID(CfgID INT);
DROP TYPE IF EXISTS SubConfigNameAndID;

CREATE TYPE SubConfigNameAndID AS (SubConfigName TEXT, SubConfigID INT);
CREATE OR REPLACE FUNCTION ListSubConfigNameAndID(CfgID INT) RETURNS SETOF SubConfigNameAndID AS $$
DECLARE
   tmp_type_and_id RECORD;
   tmp_name_and_id RECORD;
   res SubConfigNameAndID;
BEGIN
   FOR tmp_type_and_id IN SELECT SubConfigType, SubConfigID FROM MainConfigTable WHERE ConfigID = CfgID ORDER BY SubConfigType LOOP
   SELECT INTO tmp_name_and_id SubConfigName FROM ConfigLookUp WHERE SubConfigType = tmp_type_and_id.SubConfigType LIMIT 1;

   res.SubConfigName := tmp_name_and_id.SubConfigName;
   res.SubConfigID   := tmp_type_and_id.SubConfigID;
   RETURN NEXT res;
   END LOOP;
   RETURN;
END;
$$ LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION ListSubConfigNameAndID() RETURNS SETOF RECORD AS $$
       SELECT SubConfigName, SubConfigType FROM ConfigLookUp ORDER BY SubConfigType
$$ LANGUAGE SQL;


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
CREATE OR REPLACE FUNCTION InsertConfigurationSet(configtabletype text,
						  insubconfid INT,
						  incrate INT, 
						  inchannel INT,
						  columns HSTORE) RETURNS INT AS $$
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
CREATE OR REPLACE FUNCTION MaxSubConfigID(configtablename TEXT) RETURNS INT AS $$
DECLARE
   myrec RECORD;
   query TEXT;
BEGIN

   -- First find if this configuration type exists. If not, don't do anything
   IF NOT EXISTS ( SELECT TRUE FROM ConfigLookUp WHERE SubConfigName = configtablename )    
     THEN RAISE EXCEPTION '++++++++++ Configuration % is not defined yet! +++++++++++', configtablename;
   END IF;

   query := format('SELECT ConfigID FROM %s ORDER BY ConfigID DESC LIMIT 1;',configtablename);
   EXECUTE query INTO myrec;

   IF myrec.ConfigID IS NULL
       THEN RETURN -1;
   ELSE 
       RETURN myrec.ConfigID;
   END IF;
END;
$$ LANGUAGE plpgsql VOLATILE STRICT;

------------------------------------------------------------------------

DROP FUNCTION IF EXISTS ListCrateID( SCfgName TEXT, SCfgID INT);
CREATE OR REPLACE FUNCTION ListCrateID ( SCfgName TEXT,
       	  	  	               	 SCfgID   INT  ) RETURNS SETOF INT AS $$
DECLARE
   myrec RECORD;
   str_query TEXT;
   res RECORD;
BEGIN

   -- First find if this configuration type exists. If not, don't do anything
   IF NOT EXISTS ( SELECT TRUE FROM ConfigLookUp WHERE SubConfigName = SCfgName )    
     THEN RAISE EXCEPTION '++++++++++ Configuration % is not defined yet! +++++++++++', SCfgName;
   END IF;
   
   str_query := format('SELECT TRUE FROM %s WHERE ConfigID = %s LIMIT 1;',SCfgName,SCfgID);
   EXECUTE str_query INTO myrec;

   IF myrec.bool IS NULL
      THEN RAISE EXCEPTION '++++++++++ Configuration % does not have config ID % +++++++++++', SCfgName, SCfgID;
   END IF;

   str_query := format('SELECT crate FROM %s WHERE ConfigID = %s ORDER BY crate;',SCfgName,SCfgID);
   for res in EXECUTE str_query LOOP
      RETURN NEXT res.crate;
   END LOOP;
   RETURN;

END;
$$ LANGUAGE plpgsql VOLATILE STRICT;

------------------------------------------------------------------------

CREATE OR REPLACE FUNCTION InsertMainConfiguration(subconfigparameters HSTORE,confname text DEFAULT 'no_name') RETURNS INT AS $$
    DECLARE
    myrec1 RECORD;
    myrec2 RECORD;
    myrec3 RECORD;
    ColumnPair RECORD;
    SubConfigCount INT;
   -- maskconfig INT;
    newconfig INT;
    query text;  
    SubConfT INT;
    --mainconfigexists INT;
    localconfigexists INT;
    BEGIN    

    -- 1st CHECK: check if the provided Config ID already exsits in the MainConfigTable or not
    IF  (confname <> 'no_name' ) AND EXISTS (SELECT ConfigID FROM MainConfigTable WHERE MainConfigTable.ConfigName = confname)
        THEN RAISE EXCEPTION '+++++++++++++ Config with name % already exists in MainConfigTable! +++++++++++++',confname;
	RETURN -1;
    END IF;
    -- now we know that the name doesn't exist or the user doesn't care.

    -- 2nd CHECK: check if specified SubConfigType and SubConfigID are available or not
    FOR ColumnPair IN SELECT (each(subconfigparameters)).*
    LOOP
        -- Check if SubConfigType is valid or not
--        RAISE NOTICE 'key:%, value: %', ColumnPair.key, ColumnPair.value;
	IF NOT EXISTS ( SELECT TRUE FROM ConfigLookUp WHERE SubConfigName = ColumnPair.key)
	    THEN RAISE EXCEPTION '++++++++++++ Configuration % is not defined! +++++++++++++++', ColumnPair.key;
	    RETURN 1;
	END IF;
	-- Check if SubConfigID is valid or not
	query := format('SELECT TRUE FROM %s WHERE COnfigID=%s',ColumnPair.key,ColumnPair.value);
	EXECUTE query INTO myrec2;
	IF myrec2.bool IS NULL
	    THEN RAISE EXCEPTION '++++++++++++ Configuration % does not contain ConfigID=%++++++++++++',ColumnPair.key,ColumnPair.value;
	    RETURN 1;
	END IF;
	
    END LOOP;
  
    -- mainconfigexists:=1;  -- assuming it exists. Let's be proven otherwise.
    --let's check if the configuration exists:
    -- since we want to check the configuration sets, it's ok if we just choose the first subconfigtype and ID.
    -- and then we loop over all of the subsets of MainconfigTable to see if they contain all of the other ones.

    -- !!! Still missing. Find that the configuration we havefound does not have more rows than we need. !!!


    SELECT (each(subconfigparameters)).* LIMIT 1 INTO ColumnPair;  -- only use the first.
    --get subconfig type
    ------------------------------ this repeats the functionality from later.

    SELECT  SubconfigType FROM ConfigLookUp WHERE SubConfigName = ColumnPair.key INTO SubConfT;
    --looping over all mainconfigs that have the above parameters. 
    for myrec3 IN SELECT DISTINCT configID FROM MainconfigTable WHERE SubConfigType = SubConfT AND SubConfigID = CAST(ColumnPair.value AS INT)
      LOOP

         -- Report
--         RAISE NOTICE 'Checking against Main Config ID %', myrec3;

	 -- First check the # of sub-config types in this configuration. If not same as the # elements in subconfigparameters, continue.
	 SELECT COUNT(TRUE) FROM MainConfigTable WHERE ConfigID = myrec3.ConfigID INTO SubConfigCount;
--	 SubConfigCount := SubConfigCount - ARRAY_LENGTH(AKEYS(subconfigparameters));
--	 SELECT COUNT(TRUE) - SubConfigCount FROM 
--	 IF SubConfigCount = 0
	 IF NOT SubConfigCount = ARRAY_LENGTH(AKEYS(subconfigparameters),1)
	    THEN CONTINUE;
	 END IF;

         -- loop over all of the new, to be insterted config setting and check if the current configID has them all.
         localconfigexists := 1;
	 for ColumnPair IN SELECT (each(subconfigparameters)).*
	     LOOP
	     SELECT SubConfigType FROM ConfigLookUp WHERE SubConfigName = ColumnPair.key INTO SubConfT;
             IF NOT EXISTS( SELECT TRUE FROM MainconfigTable WHERE SubConfigType = SubConfT AND subconfigid=CAST(ColumnPair.value AS INT) AND ConfigID=myrec3.ConfigID )
		  THEN localconfigexists := 0;
		  EXIT;   -- break out of loop. We already know, this is not the right config.
	     END IF;
             END LOOP;
         IF localconfigexists = 1    -- we've gone through all of the PMT settings and all exist.
	    THEN RAISE EXCEPTION '+++++++++++++ This Configuration exists MainConfigTable, with ID: %! +++++++++++++',myrec3.ConfigID;
	    RETURN -1;
	 END IF;
     END LOOP;  -- end of myrec3 FOR loop over all of the subconfigs


--     IF mainconfigexists = 1
--       THEN RAISE EXCEPTION '+++++++++++++ Config already exists in MainConfigTable, with ID: %! +++++++++++++',-1;
--     END IF;

    ----------- To be improved

    --------------------------------- Find the last entry
    SELECT configID FROM MainConfigTable ORDER BY configID DESC LIMIT 1 INTO newconfig ;
    IF newconfig IS NULL
      THEN  newconfig := 0;
    ELSE
       newconfig := newconfig + 1;
    END IF;
    
    RAISE NOTICE 'Inserting a new entry in MainConfigTable: Name=% ... ID=%',confname,newconfig;
   
    -- Reaching this point means input values are valid. Let's insert.
   for myrec1 IN SELECT (each(subconfigparameters)).*
    LOOP
    SELECT SubconfigType FROM ConfigLookUp WHERE SubConfigName = myrec1.key INTO SubConfT;
    INSERT INTO MainconfigTable (ConfigID,
    	   			 SubConfigType,
				 SubConfigID, 
				 SubConfigOnMask,
				 ConfigName) VALUES(newconfig,SubConfT,CAST(myrec1.value AS INT),65636,confname);

    END LOOP;

   RETURN newconfig;
   END;
$$ LANGUAGE plpgsql VOLATILE STRICT;


------------------------------------------------------------------------

CREATE OR REPLACE FUNCTION CleanConfigDB() RETURNS INT AS $$
DECLARE
  ConfigTable RECORD;
  query TEXT;
BEGIN
    DELETE FROM MainConfigTable;
    FOR ConfigTable IN SELECT DISTINCT SubConfigName FROM ConfigLookUp
    LOOP
        query := 'DROP TABLE IF EXISTS '||ConfigTable.SubConfigName||';';
	EXECUTE query;
    END LOOP;
    DELETE FROM ConfigLookUp;
    RETURN 0;
END;
$$ LANGUAGE plpgsql VOLATILE STRICT;


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

      mainconfquery :='SELECT  * FROM mainconfigtable WHERE subconfigtype=' || conftype || ' AND configid='||mainconfigid;  

      RAISE INFO ' % ', mainconfquery ;
   
      EXECUTE mainconfquery INTO mainconfrow;

      RAISE INFO 'conf: %  , subconf: %   ,  parent: %  ', mainconfrow.configid, mainconfrow.subconfigid ,  mainconfrow.subconfigparentid ;
  
  ---- SELECT   
      query := 'SELECT * FROM ' || tablename || ' WHERE ' || tablename || '.configid =' || mainconfrow.subconfigparentid ;

      for subconfigconfigrec in EXECUTE query loop
      subconfigconfigrec.ConfigName:=tablename;  --inserting the name for faster lookup later
      subconfigconfigrec.ConfigType:=conftype;   --inserting type for faster lookup

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




















