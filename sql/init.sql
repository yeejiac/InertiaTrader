-- --------------------------------------------------------
-- 主機:                           192.168.56.104
-- 伺服器版本:                        10.1.48-MariaDB - MariaDB Server
-- 伺服器作業系統:                      Linux
-- HeidiSQL 版本:                  11.0.0.5919
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;


-- 傾印 account_Info 的資料庫結構
CREATE DATABASE IF NOT EXISTS `account_Info` /*!40100 DEFAULT CHARACTER SET latin1 */;
USE `account_Info`;

-- 傾印  資料表 account_Info.Inventory 結構
CREATE TABLE IF NOT EXISTS `Inventory` (
  `username` int(11) NOT NULL,
  `side` int(11) NOT NULL,
  `price` double NOT NULL DEFAULT '0',
  `product` varchar(50) NOT NULL DEFAULT '',
  `datetime` varchar(50) NOT NULL DEFAULT '',
  `NID` float NOT NULL,
  PRIMARY KEY (`username`,`NID`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- 正在傾印表格  account_Info.Inventory 的資料：~0 rows (近似值)
/*!40000 ALTER TABLE `Inventory` DISABLE KEYS */;
/*!40000 ALTER TABLE `Inventory` ENABLE KEYS */;


-- 傾印 stock 的資料庫結構
CREATE DATABASE IF NOT EXISTS `stock` /*!40100 DEFAULT CHARACTER SET latin1 */;
USE `stock`;

-- 傾印  資料表 stock.ExecReport 結構
CREATE TABLE IF NOT EXISTS `ExecReport` (
  `NID` float NOT NULL,
  `OrderPrice` double NOT NULL,
  `ExecPrice` double(22,0) NOT NULL,
  `Side` int(1) NOT NULL,
  `Client_SerialNum` bigint(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`NID`,`Side`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- 正在傾印表格  stock.ExecReport 的資料：~0 rows (近似值)
/*!40000 ALTER TABLE `ExecReport` DISABLE KEYS */;
/*!40000 ALTER TABLE `ExecReport` ENABLE KEYS */;

-- 傾印  資料表 stock.InitData 結構
CREATE TABLE IF NOT EXISTS `InitData` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `Date` varchar(50) NOT NULL DEFAULT '0' COMMENT '日期',
  `Price_Now` double(22,0) NOT NULL DEFAULT '0' COMMENT '現價',
  `Price_Start` double NOT NULL COMMENT '開盤價',
  `Price_End` double NOT NULL COMMENT '收盤價',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;

-- 正在傾印表格  stock.InitData 的資料：~0 rows (近似值)
/*!40000 ALTER TABLE `InitData` DISABLE KEYS */;
INSERT INTO `InitData` (`ID`, `Date`, `Price_Now`, `Price_Start`, `Price_End`) VALUES
	(1, '20210425', 60, 60.5, 60.5);
/*!40000 ALTER TABLE `InitData` ENABLE KEYS */;

-- 傾印  資料表 stock.Order 結構
CREATE TABLE IF NOT EXISTS `Order` (
  `NID` float NOT NULL DEFAULT '0',
  `OrderPrice` double NOT NULL,
  `Symbol` varchar(50) NOT NULL,
  `UserID` float NOT NULL,
  `Side` int(1) NOT NULL,
  `Order_situation` int(1) NOT NULL COMMENT '1 = 委託中，2 = 成交, 3 = 刪單',
  `Client_SerialNum` bigint(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`NID`,`Client_SerialNum`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- 正在傾印表格  stock.Order 的資料：~0 rows (近似值)
/*!40000 ALTER TABLE `Order` DISABLE KEYS */;
/*!40000 ALTER TABLE `Order` ENABLE KEYS */;

-- 傾印  資料表 stock.ProductList 結構
CREATE TABLE IF NOT EXISTS `ProductList` (
  `product_id` varchar(50) NOT NULL,
  `transaction_flag` enum('Y','N') NOT NULL,
  `note` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`product_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- 正在傾印表格  stock.ProductList 的資料：~0 rows (近似值)
/*!40000 ALTER TABLE `ProductList` DISABLE KEYS */;
INSERT INTO `ProductList` (`product_id`, `transaction_flag`, `note`) VALUES
	('KKC', 'Y', 'kow kow coin');
/*!40000 ALTER TABLE `ProductList` ENABLE KEYS */;

-- 傾印  資料表 stock.User 結構
CREATE TABLE IF NOT EXISTS `User` (
  `user` varchar(50) NOT NULL,
  `password` varchar(50) NOT NULL,
  `balance` double(22,0) NOT NULL COMMENT '投入資產價值(單位KKC)',
  `book_value` double NOT NULL COMMENT '帳面價值',
  PRIMARY KEY (`user`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- 正在傾印表格  stock.User 的資料：~1 rows (近似值)
/*!40000 ALTER TABLE `User` DISABLE KEYS */;
INSERT INTO `User` (`user`, `password`, `balance`, `book_value`) VALUES
	('0324027', '123', 1000000, 1000000);
/*!40000 ALTER TABLE `User` ENABLE KEYS */;

-- 傾印  檢視 stock.Valid_Order 結構
-- 建立臨時表格，以解決檢視依存性錯誤
CREATE TABLE `Valid_Order` (
	`NID` FLOAT(12) NOT NULL,
	`OrderPrice` DOUBLE(22,0) NOT NULL,
	`Symbol` VARCHAR(50) NOT NULL COLLATE 'latin1_swedish_ci',
	`UserID` FLOAT(12) NOT NULL,
	`Side` INT(1) NOT NULL,
	`Order_situation` INT(1) NOT NULL COMMENT '1 = 委託中，2 = 成交, 3 = 刪單',
	`Client_SerialNum` BIGINT(20) NOT NULL
) ENGINE=MyISAM;

-- 傾印  檢視 stock.Valid_Order 結構
-- 移除臨時表格，並建立最終檢視結構
DROP TABLE IF EXISTS `Valid_Order`;
CREATE ALGORITHM=UNDEFINED SQL SECURITY DEFINER VIEW `Valid_Order` AS select `Order`.`NID` AS `NID`,`Order`.`OrderPrice` AS `OrderPrice`,`Order`.`Symbol` AS `Symbol`,`Order`.`UserID` AS `UserID`,`Order`.`Side` AS `Side`,`Order`.`Order_situation` AS `Order_situation`,`Order`.`Client_SerialNum` AS `Client_SerialNum` from `Order` where (`Order`.`Order_situation` = '1');

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
