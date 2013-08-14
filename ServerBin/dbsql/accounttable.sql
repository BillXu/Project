delimiter $$

CREATE TABLE `account` (
  `Account` varchar(50) NOT NULL,
  `Password` varchar(45) NOT NULL,
  `CharacterName` varchar(45) NOT NULL,
  `Sex` tinyint(4) NOT NULL DEFAULT '0',
  `UserUID` int(10) unsigned NOT NULL,
  PRIMARY KEY (`Account`,`UserUID`),
  UNIQUE KEY `UserUID_UNIQUE` (`UserUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8$$

