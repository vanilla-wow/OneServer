ALTER TABLE realmd_dev_db_version CHANGE COLUMN required_dev_0004_01_realmd_dev_db_version required_dev_0005_01_realmd_autobroadcast BIT;

CREATE TABLE IF NOT EXISTS `autobroadcast` (
  `id` INT(11) NOT NULL AUTO_INCREMENT,
  `text` LONGTEXT NOT NULL,
  `next` INT(11) NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MYISAM  DEFAULT CHARSET=utf8;
