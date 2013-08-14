delimiter $$

CREATE TABLE `account` (
  `Account` varchar(50) NOT NULL,
  `Password` varchar(45) NOT NULL,
  `CharacterName` varchar(45) NOT NULL,
  `Sex` tinyint(4) NOT NULL DEFAULT '0',
  `UserUID` int(10) unsigned NOT NULL,
  `nCoin` int(10) unsigned DEFAULT '50000',
  `nDiamoned` int(10) unsigned zerofill NOT NULL,
  `nTitle` tinyint(3) unsigned zerofill NOT NULL,
  `nVipLevel` tinyint(3) unsigned zerofill NOT NULL,
  `nSex` tinyint(3) unsigned zerofill NOT NULL,
  `nDefaulPhotoID` smallint(5) unsigned DEFAULT '1',
  `nUserDefinePhotoID` int(10) unsigned zerofill DEFAULT NULL,
  `nYeastodayWinCoin` int(10) unsigned zerofill NOT NULL,
  `nSingleWinMost` int(10) unsigned zerofill NOT NULL,
  `nWinTimes` smallint(5) unsigned zerofill NOT NULL,
  `nLoseTimes` smallint(5) unsigned zerofill NOT NULL,
  `nQQNumber` int(10) unsigned zerofill NOT NULL,
  `strSigure` varchar(145) DEFAULT NULL,
  PRIMARY KEY (`Account`,`UserUID`),
  UNIQUE KEY `UserUID_UNIQUE` (`UserUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8$$

