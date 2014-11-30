ALTER TABLE dev_db_version CHANGE COLUMN required_dev_0006_02_mangos_command required_dev_0007_01_mangos_instance_template BIT;

ALTER TABLE `instance_template` DROP COLUMN `allowMount`;
