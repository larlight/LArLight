
DROP FUNCTION IF EXISTS TestFunc();

CREATE OR REPLACE FUNCTION TestFunc(mainconfID INT, columns HSTORE) RETURNS INT AS $$
DECLARE
  myrec1 RECORD;
  myrec2 RECORD;
  query TEXT;
BEGIN

    -- First check if the provided Config ID already exsits in the MainConfigTable or not
    IF EXISTS (SELECT TRUE FROM MainConfigTable WHERE ConfigID = mainconfID)
        THEN RAISE EXCEPTION '+++++++++++++ Config ID % already exists in MainConfigTable! +++++++++++++',mainconfID;
	RETURN 1;
    END IF;
    
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
    
    -- Reaching this point means input values are valid. Let's insert.
    

    -- Success. Return 0.
    RETURN 0;
END

$$ LANGUAGE plpgsql VOLATILE STRICT;


