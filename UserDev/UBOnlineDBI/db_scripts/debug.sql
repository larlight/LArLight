
-- Sample script to see how things run

-- Clean Up
SELECT CleanConfigDB();

-- Test1 Sub-Configuration
SELECT CreateConfigurationType('Test1','test1=>1');
SELECT InsertConfigurationSet('Test1',0,0,0,'test1=>1');
SELECT InsertConfigurationSet('Test1',1,0,0,'test1=>1');
SELECT InsertConfigurationSet('Test1',2,0,0,'test1=>1');

-- Test2 Sub-Configuration
SELECT CreateConfigurationType('Test2','test2=>1');
SELECT InsertConfigurationSet('Test2',0,0,0,'test2=>1');
SELECT InsertConfigurationSet('Test2',1,0,0,'test2=>1');
SELECT InsertConfigurationSet('Test2',2,0,0,'test2=>1');

-- Test3 Sub-Configuration
SELECT CreateConfigurationType('Test3','test3=>1');
SELECT InsertConfigurationSet('Test3',0,0,0,'test3=>1');
SELECT InsertConfigurationSet('Test3',1,0,0,'test3=>1');
SELECT InsertConfigurationSet('Test3',2,0,0,'test3=>1');

-- Let's try some exception calls
--SELECT InsertConfigurationSet('Test1',0,0,0,'test1=>1');

-- Create MainConfiguration
SELECT InsertMainConfiguration('Test1=>0,Test2=>0,Test3=>0'::hstore,'MainTest1');
-- Attempt to re-use the same name => Should have an exception
SELECT InsertMainConfiguration('Test1=>0,Test2=>0,Test3=>0'::hstore,'MainTest1');
-- Attempt to insert the identical configuration set with different name => Should have an exception
SELECT InsertMainConfiguration('Test1=>0,Test2=>0,Test3=>0'::hstore,'MainTest2');
-- Attempt to insert different configuration with different name => Should not have an exception
SELECT InsertMainConfiguration('Test1=>1,Test2=>0,Test3=>0'::hstore,'MainTest2');
-- Attempt to insert "similar" configuration (lacking one of sub-config otherwise identical) => Should not have an exception
SELECT InsertMainConfiguration('Test1=>1,Test2=>0'::hstore,'MainTest3');



