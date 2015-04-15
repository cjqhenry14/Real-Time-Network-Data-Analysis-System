-- phpMyAdmin SQL Dump
-- version 4.0.9
-- http://www.phpmyadmin.net
--
-- 主机: localhost
-- 生成日期: 2014-06-07 09:22:40
-- 服务器版本: 5.5.34
-- PHP 版本: 5.4.22

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- 数据库: `myIDS`
--

-- --------------------------------------------------------

--
-- 表的结构 `attackStore`
--

CREATE TABLE IF NOT EXISTS `attackStore` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `type` int(2) NOT NULL,
  `src_ip` varchar(20) COLLATE utf8mb4_unicode_ci NOT NULL,
  `dst_ip` varchar(20) COLLATE utf8mb4_unicode_ci NOT NULL,
  `src_port` int(11) NOT NULL,
  `dst_port` int(11) NOT NULL,
  `rule_id` int(11) NOT NULL,
  `attact_time` varchar(50) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci AUTO_INCREMENT=376 ;

--
-- 转存表中的数据 `attackStore`
--

INSERT INTO `attackStore` (`id`, `type`, `src_ip`, `dst_ip`, `src_port`, `dst_port`, `rule_id`, `attact_time`) VALUES
(1, 0, '74.125.235.193', '192.168.1.107', 443, 43947, 1662, 'Sat Apr 12 19:02:16 2014\n'),
(2, 0, '211.151.247.92', '192.168.1.107', 80, 42964, 626, 'Sat Apr 12 19:02:16 2014\n'),
(3, 0, '211.151.247.92', '192.168.1.107', 80, 42964, 1674, 'Sat Apr 12 19:02:16 2014\n'),
(4, 0, '211.151.247.92', '192.168.1.107', 80, 42964, 1349, 'Sat Apr 12 19:02:16 2014\n'),
(5, 0, '211.151.247.92', '192.168.1.107', 80, 42964, 1674, 'Sat Apr 12 19:02:16 2014\n'),
(6, 0, '211.151.247.38', '192.168.1.107', 80, 53384, 626, 'Sat Apr 12 19:02:16 2014\n'),
(7, 0, '211.151.247.38', '192.168.1.107', 80, 53384, 86, 'Sat Apr 12 19:02:16 2014\n'),
(8, 0, '211.151.247.38', '192.168.1.107', 80, 53384, 1674, 'Sat Apr 12 19:02:16 2014\n'),
(9, 0, '211.151.247.38', '192.168.1.107', 80, 53384, 86, 'Sat Apr 12 19:02:16 2014\n'),
(10, 0, '211.151.247.38', '192.168.1.107', 80, 53384, 1674, 'Sat Apr 12 19:02:16 2014\n'),
(11, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 626, 'Sat Apr 12 19:02:16 2014\n'),
(12, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1315, 'Sat Apr 12 19:02:16 2014\n'),
(13, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1674, 'Sat Apr 12 19:02:16 2014\n'),
(14, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1383, 'Sat Apr 12 19:02:16 2014\n'),
(15, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1685, 'Sat Apr 12 19:02:16 2014\n'),
(16, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1685, 'Sat Apr 12 19:02:16 2014\n'),
(17, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1685, 'Sat Apr 12 19:02:16 2014\n'),
(18, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1685, 'Sat Apr 12 19:02:16 2014\n'),
(19, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1744, 'Sat Apr 12 19:02:16 2014\n'),
(20, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1685, 'Sat Apr 12 19:02:16 2014\n'),
(21, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1685, 'Sat Apr 12 19:02:16 2014\n'),
(22, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1744, 'Sat Apr 12 19:02:16 2014\n'),
(23, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1685, 'Sat Apr 12 19:02:16 2014\n'),
(24, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1744, 'Sat Apr 12 19:02:16 2014\n'),
(25, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1296, 'Sat Apr 12 19:02:16 2014\n'),
(26, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1685, 'Sat Apr 12 19:02:16 2014\n'),
(27, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1075, 'Sat Apr 12 19:02:16 2014\n'),
(28, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1685, 'Sat Apr 12 19:02:16 2014\n'),
(29, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1342, 'Sat Apr 12 19:02:16 2014\n'),
(30, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1342, 'Sat Apr 12 19:02:16 2014\n'),
(31, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1342, 'Sat Apr 12 19:02:16 2014\n'),
(32, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1744, 'Sat Apr 12 19:02:16 2014\n'),
(33, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1389, 'Sat Apr 12 19:02:16 2014\n'),
(34, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1389, 'Sat Apr 12 19:02:16 2014\n'),
(35, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1296, 'Sat Apr 12 19:02:16 2014\n'),
(36, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1419, 'Sat Apr 12 19:02:16 2014\n'),
(37, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 525, 'Sat Apr 12 19:02:16 2014\n'),
(38, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1744, 'Sat Apr 12 19:02:16 2014\n'),
(39, 1, '192.168.1.1', '192.168.1.107', 53, 42935, 46, 'Sat Apr 12 19:02:16 2014\n'),
(40, 1, '192.168.1.1', '192.168.1.107', 53, 17418, 46, 'Sat Apr 12 19:02:16 2014\n'),
(41, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 525, 'Sat Apr 12 19:02:16 2014\n'),
(42, 1, '192.168.1.1', '192.168.1.107', 53, 40125, 46, 'Sat Apr 12 19:02:16 2014\n'),
(43, 1, '192.168.1.1', '192.168.1.107', 53, 40125, 46, 'Sat Apr 12 19:02:16 2014\n'),
(44, 1, '192.168.1.1', '192.168.1.107', 53, 40125, 46, 'Sat Apr 12 19:02:16 2014\n'),
(45, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 784, 'Sat Apr 12 19:02:16 2014\n'),
(46, 1, '192.168.1.1', '192.168.1.107', 53, 40125, 46, 'Sat Apr 12 19:02:16 2014\n'),
(47, 1, '192.168.1.1', '192.168.1.107', 53, 44685, 46, 'Sat Apr 12 19:02:16 2014\n'),
(48, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 525, 'Sat Apr 12 19:02:16 2014\n'),
(49, 1, '192.168.1.1', '192.168.1.107', 53, 56609, 1383, 'Sat Apr 12 19:02:16 2014\n'),
(50, 1, '192.168.1.1', '192.168.1.107', 53, 22279, 46, 'Sat Apr 12 19:02:16 2014\n'),
(51, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1685, 'Sat Apr 12 19:02:16 2014\n'),
(52, 1, '192.168.1.1', '192.168.1.107', 53, 5828, 46, 'Sat Apr 12 19:02:16 2014\n'),
(53, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1419, 'Sat Apr 12 19:02:16 2014\n'),
(54, 1, '192.168.1.1', '192.168.1.107', 53, 37045, 46, 'Sat Apr 12 19:02:16 2014\n'),
(55, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1389, 'Sat Apr 12 19:02:16 2014\n'),
(56, 1, '192.168.1.1', '192.168.1.107', 53, 37045, 46, 'Sat Apr 12 19:02:16 2014\n'),
(57, 1, '192.168.1.1', '192.168.1.107', 53, 37045, 46, 'Sat Apr 12 19:02:16 2014\n'),
(58, 1, '192.168.1.1', '192.168.1.107', 53, 37045, 46, 'Sat Apr 12 19:02:16 2014\n'),
(59, 0, '112.125.123.23', '192.168.1.107', 80, 40848, 1349, 'Sat Apr 12 19:02:16 2014\n'),
(60, 0, '112.125.123.23', '192.168.1.107', 80, 40848, 626, 'Sat Apr 12 19:02:16 2014\n'),
(61, 1, '192.168.1.1', '192.168.1.107', 53, 37045, 46, 'Sat Apr 12 19:02:16 2014\n'),
(62, 0, '112.125.123.23', '192.168.1.107', 80, 40848, 2057, 'Sat Apr 12 19:02:16 2014\n'),
(63, 1, '192.168.1.1', '192.168.1.107', 53, 37045, 46, 'Sat Apr 12 19:02:16 2014\n'),
(64, 0, '112.125.123.23', '192.168.1.107', 80, 40848, 1062, 'Sat Apr 12 19:02:16 2014\n'),
(65, 1, '192.168.1.1', '192.168.1.107', 53, 59497, 46, 'Sat Apr 12 19:02:16 2014\n'),
(66, 0, '112.125.123.23', '192.168.1.107', 80, 40848, 1349, 'Sat Apr 12 19:02:16 2014\n'),
(67, 1, '192.168.1.1', '192.168.1.107', 53, 34556, 1383, 'Sat Apr 12 19:02:16 2014\n'),
(68, 0, '112.125.123.23', '192.168.1.107', 80, 40848, 790, 'Sat Apr 12 19:02:16 2014\n'),
(69, 1, '192.168.1.1', '192.168.1.107', 53, 25613, 1383, 'Sat Apr 12 19:02:16 2014\n'),
(70, 0, '112.125.123.23', '192.168.1.107', 80, 40848, 1419, 'Sat Apr 12 19:02:16 2014\n'),
(71, 1, '192.168.1.1', '192.168.1.107', 53, 50438, 46, 'Sat Apr 12 19:02:16 2014\n'),
(72, 1, '192.168.1.1', '192.168.1.107', 53, 16561, 1383, 'Sat Apr 12 19:02:16 2014\n'),
(73, 1, '192.168.1.1', '192.168.1.107', 53, 10718, 46, 'Sat Apr 12 19:02:16 2014\n'),
(74, 1, '192.168.1.1', '192.168.1.107', 53, 10718, 46, 'Sat Apr 12 19:02:16 2014\n'),
(75, 1, '192.168.1.1', '192.168.1.107', 53, 10718, 46, 'Sat Apr 12 19:02:16 2014\n'),
(76, 1, '192.168.1.1', '192.168.1.107', 53, 24955, 1383, 'Sat Apr 12 19:02:16 2014\n'),
(77, 0, '112.125.123.23', '192.168.1.107', 80, 40855, 626, 'Sat Apr 12 19:02:16 2014\n'),
(78, 0, '112.125.123.23', '192.168.1.107', 80, 40855, 2057, 'Sat Apr 12 19:02:16 2014\n'),
(79, 0, '112.125.123.23', '192.168.1.107', 80, 40855, 1062, 'Sat Apr 12 19:02:16 2014\n'),
(80, 0, '112.125.123.23', '192.168.1.107', 80, 40856, 626, 'Sat Apr 12 19:02:16 2014\n'),
(81, 0, '112.125.123.23', '192.168.1.107', 80, 40856, 2057, 'Sat Apr 12 19:02:16 2014\n'),
(82, 0, '112.125.123.23', '192.168.1.107', 80, 40856, 1062, 'Sat Apr 12 19:02:16 2014\n'),
(83, 0, '112.125.123.23', '192.168.1.107', 80, 40856, 1030, 'Sat Apr 12 19:02:16 2014\n'),
(84, 0, '112.125.123.23', '192.168.1.107', 80, 40856, 517, 'Sat Apr 12 19:02:16 2014\n'),
(85, 0, '203.208.48.137', '192.168.1.107', 80, 53211, 2057, 'Sat Apr 12 19:02:16 2014\n'),
(86, 0, '203.208.48.137', '192.168.1.107', 80, 53211, 626, 'Sat Apr 12 19:02:16 2014\n'),
(87, 0, '67.215.253.139', '192.168.1.107', 80, 56831, 982, 'Sat Apr 12 19:02:16 2014\n'),
(88, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 1701, 'Sun Apr 13 13:56:52 2014\n'),
(89, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 606, 'Sun Apr 13 13:56:52 2014\n'),
(90, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 87, 'Sun Apr 13 13:56:52 2014\n'),
(91, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 42, 'Sun Apr 13 13:56:52 2014\n'),
(92, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 87, 'Sun Apr 13 13:56:52 2014\n'),
(93, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 1701, 'Sun Apr 13 13:56:52 2014\n'),
(94, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 606, 'Sun Apr 13 13:56:52 2014\n'),
(95, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 42, 'Sun Apr 13 13:56:52 2014\n'),
(96, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 1701, 'Sun Apr 13 13:56:52 2014\n'),
(97, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 606, 'Sun Apr 13 13:56:52 2014\n'),
(98, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 42, 'Sun Apr 13 13:56:52 2014\n'),
(99, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 1701, 'Sun Apr 13 13:56:52 2014\n'),
(100, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 606, 'Sun Apr 13 13:56:52 2014\n'),
(101, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 42, 'Sun Apr 13 13:56:52 2014\n'),
(102, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 1701, 'Sun Apr 13 13:56:52 2014\n'),
(103, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 606, 'Sun Apr 13 13:56:52 2014\n'),
(104, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 87, 'Sun Apr 13 13:56:52 2014\n'),
(105, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 42, 'Sun Apr 13 13:56:52 2014\n'),
(106, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 87, 'Sun Apr 13 13:56:52 2014\n'),
(107, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 1701, 'Sun Apr 13 13:56:52 2014\n'),
(108, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 606, 'Sun Apr 13 13:56:52 2014\n'),
(109, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 42, 'Sun Apr 13 13:56:52 2014\n'),
(110, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 1701, 'Sun Apr 13 13:56:52 2014\n'),
(111, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 606, 'Sun Apr 13 13:56:52 2014\n'),
(112, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 42, 'Sun Apr 13 13:56:52 2014\n'),
(113, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 1701, 'Sun Apr 13 13:56:52 2014\n'),
(114, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 606, 'Sun Apr 13 13:56:52 2014\n'),
(115, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 42, 'Sun Apr 13 13:56:52 2014\n'),
(116, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 1701, 'Sun Apr 13 13:56:52 2014\n'),
(117, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 606, 'Sun Apr 13 13:56:52 2014\n'),
(118, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 42, 'Sun Apr 13 13:56:52 2014\n'),
(119, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 1701, 'Sun Apr 13 13:56:52 2014\n'),
(120, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 606, 'Sun Apr 13 13:56:52 2014\n'),
(121, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 42, 'Sun Apr 13 13:56:52 2014\n'),
(122, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 1701, 'Sun Apr 13 13:56:52 2014\n'),
(123, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 606, 'Sun Apr 13 13:56:52 2014\n'),
(124, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 42, 'Sun Apr 13 13:56:52 2014\n'),
(125, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 1701, 'Sun Apr 13 13:56:52 2014\n'),
(126, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 606, 'Sun Apr 13 13:56:52 2014\n'),
(127, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 42, 'Sun Apr 13 13:56:52 2014\n'),
(128, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 1701, 'Sun Apr 13 13:56:52 2014\n'),
(129, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 606, 'Sun Apr 13 13:56:52 2014\n'),
(130, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 42, 'Sun Apr 13 13:56:52 2014\n'),
(131, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 1701, 'Sun Apr 13 13:56:52 2014\n'),
(132, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 606, 'Sun Apr 13 13:56:52 2014\n'),
(133, 1, '192.168.1.1', '192.168.1.110', 1025, 1900, 42, 'Sun Apr 13 13:56:52 2014\n'),
(134, 1, '192.168.1.105', '192.168.1.110', 57012, 1900, 990, 'Sun Apr 13 13:56:52 2014\n'),
(135, 1, '192.168.1.105', '192.168.1.110', 1900, 1900, 42, 'Sun Apr 13 13:56:52 2014\n'),
(136, 1, '192.168.1.105', '192.168.1.110', 1900, 1900, 8, 'Sun Apr 13 13:56:52 2014\n'),
(137, 1, '192.168.1.105', '192.168.1.110', 1900, 1900, 42, 'Sun Apr 13 13:56:52 2014\n'),
(138, 1, '192.168.1.105', '192.168.1.110', 1900, 1900, 8, 'Sun Apr 13 13:56:52 2014\n'),
(139, 1, '192.168.1.105', '192.168.1.110', 1900, 1900, 42, 'Sun Apr 13 13:56:52 2014\n'),
(140, 1, '192.168.1.105', '192.168.1.110', 1900, 1900, 8, 'Sun Apr 13 13:56:52 2014\n'),
(141, 1, '192.168.1.105', '192.168.1.110', 1900, 1900, 42, 'Sun Apr 13 13:56:52 2014\n'),
(142, 1, '192.168.1.105', '192.168.1.110', 1900, 1900, 8, 'Sun Apr 13 13:56:52 2014\n'),
(143, 1, '192.168.1.105', '192.168.1.110', 1900, 1900, 87, 'Sun Apr 13 13:56:52 2014\n'),
(144, 1, '192.168.1.105', '192.168.1.110', 1900, 1900, 42, 'Sun Apr 13 13:56:52 2014\n'),
(145, 1, '192.168.1.105', '192.168.1.110', 1900, 1900, 8, 'Sun Apr 13 13:56:52 2014\n'),
(146, 1, '192.168.1.105', '192.168.1.110', 1900, 1900, 87, 'Sun Apr 13 13:56:52 2014\n'),
(147, 1, '192.168.1.105', '192.168.1.110', 1900, 1900, 42, 'Sun Apr 13 13:56:52 2014\n'),
(148, 1, '192.168.1.105', '192.168.1.110', 1900, 1900, 8, 'Sun Apr 13 13:56:52 2014\n'),
(149, 1, '192.168.1.105', '192.168.1.110', 1900, 1900, 42, 'Sun Apr 13 13:56:52 2014\n'),
(150, 1, '192.168.1.105', '192.168.1.110', 1900, 1900, 8, 'Sun Apr 13 13:56:52 2014\n'),
(151, 1, '192.168.1.105', '192.168.1.110', 1900, 1900, 42, 'Sun Apr 13 13:56:52 2014\n'),
(152, 1, '192.168.1.105', '192.168.1.110', 1900, 1900, 8, 'Sun Apr 13 13:56:52 2014\n'),
(153, 1, '192.168.1.105', '192.168.1.110', 1900, 1900, 42, 'Sun Apr 13 13:56:52 2014\n'),
(154, 1, '192.168.1.105', '192.168.1.110', 1900, 1900, 8, 'Sun Apr 13 13:56:52 2014\n'),
(155, 1, '192.168.1.105', '192.168.1.110', 1900, 1900, 42, 'Sun Apr 13 13:56:52 2014\n'),
(156, 1, '192.168.1.105', '192.168.1.110', 1900, 1900, 8, 'Sun Apr 13 13:56:52 2014\n'),
(157, 1, '192.168.1.105', '192.168.1.110', 1900, 1900, 42, 'Sun Apr 13 13:56:52 2014\n'),
(158, 1, '192.168.1.105', '192.168.1.110', 1900, 1900, 8, 'Sun Apr 13 13:56:52 2014\n'),
(159, 1, '192.168.1.105', '192.168.1.110', 50707, 1900, 42, 'Sun Apr 13 13:56:52 2014\n'),
(160, 1, '192.168.1.105', '192.168.1.110', 50707, 1900, 606, 'Sun Apr 13 13:56:52 2014\n'),
(161, 0, '60.28.212.42', '192.168.1.110', 80, 56186, 790, 'Sun Apr 13 13:56:52 2014\n'),
(162, 1, '192.168.1.1', '192.168.1.110', 53, 26666, 46, 'Sun Apr 13 13:56:52 2014\n'),
(163, 0, '60.28.212.42', '192.168.1.110', 80, 56186, 42, 'Sun Apr 13 13:56:52 2014\n'),
(164, 1, '192.168.1.1', '192.168.1.110', 53, 26666, 46, 'Sun Apr 13 13:56:52 2014\n'),
(165, 1, '192.168.1.1', '192.168.1.110', 53, 26666, 46, 'Sun Apr 13 13:56:52 2014\n'),
(166, 1, '192.168.1.1', '192.168.1.110', 53, 26666, 46, 'Sun Apr 13 13:56:52 2014\n'),
(167, 1, '192.168.1.1', '192.168.1.110', 53, 63732, 46, 'Sun Apr 13 13:56:52 2014\n'),
(168, 1, '192.168.1.1', '192.168.1.110', 53, 30122, 1383, 'Sun Apr 13 13:56:52 2014\n'),
(169, 0, '123.125.38.240', '192.168.1.110', 80, 51315, 790, 'Sun Apr 13 13:56:52 2014\n'),
(170, 0, '123.125.38.240', '192.168.1.110', 80, 51315, 42, 'Sun Apr 13 13:56:52 2014\n'),
(171, 0, '123.125.38.240', '192.168.1.110', 80, 51315, 2057, 'Sun Apr 13 13:56:52 2014\n'),
(172, 0, '123.125.38.240', '192.168.1.110', 80, 51315, 114, 'Sun Apr 13 13:56:52 2014\n'),
(173, 0, '123.125.38.240', '192.168.1.110', 80, 51315, 525, 'Sun Apr 13 13:56:52 2014\n'),
(174, 0, '123.125.38.240', '192.168.1.110', 80, 51315, 114, 'Sun Apr 13 13:56:52 2014\n'),
(175, 1, '192.168.1.1', '192.168.1.110', 53, 1964, 1383, 'Sun Apr 13 13:56:52 2014\n'),
(176, 0, '125.39.19.91', '192.168.1.110', 80, 49364, 1772, 'Sun Apr 13 13:56:52 2014\n'),
(177, 0, '125.39.19.91', '192.168.1.110', 80, 49364, 42, 'Sun Apr 13 13:56:52 2014\n'),
(178, 0, '125.39.19.91', '192.168.1.110', 80, 49365, 1772, 'Sun Apr 13 13:56:52 2014\n'),
(179, 0, '125.39.19.91', '192.168.1.110', 80, 49365, 42, 'Sun Apr 13 13:56:52 2014\n'),
(180, 0, '125.39.19.91', '192.168.1.110', 80, 49366, 1772, 'Sun Apr 13 13:56:52 2014\n'),
(181, 0, '125.39.19.91', '192.168.1.110', 80, 49366, 42, 'Sun Apr 13 13:56:52 2014\n'),
(182, 0, '125.39.19.91', '192.168.1.110', 80, 49368, 1772, 'Sun Apr 13 13:56:52 2014\n'),
(183, 0, '125.39.19.91', '192.168.1.110', 80, 49368, 42, 'Sun Apr 13 13:56:52 2014\n'),
(184, 0, '125.39.19.91', '192.168.1.110', 80, 49367, 1772, 'Sun Apr 13 13:56:52 2014\n'),
(185, 0, '125.39.19.91', '192.168.1.110', 80, 49367, 42, 'Sun Apr 13 13:56:52 2014\n'),
(186, 0, '123.125.38.240', '192.168.1.110', 80, 51315, 1389, 'Sun Apr 13 13:56:52 2014\n'),
(187, 0, '123.125.38.240', '192.168.1.110', 80, 51315, 1685, 'Sun Apr 13 13:56:52 2014\n'),
(188, 0, '123.125.38.240', '192.168.1.110', 80, 51315, 1419, 'Sun Apr 13 13:56:52 2014\n'),
(189, 0, '123.129.232.232', '192.168.1.110', 80, 57777, 42, 'Sun Apr 13 13:56:52 2014\n'),
(190, 1, '192.168.1.109', '192.168.1.110', 137, 137, 93, 'Sun Apr 13 13:56:52 2014\n'),
(191, 1, '192.168.1.1', '192.168.1.110', 53, 51168, 1383, 'Sun Apr 13 13:56:52 2014\n'),
(192, 0, '125.39.19.91', '192.168.1.110', 80, 49367, 1772, 'Sun Apr 13 13:56:52 2014\n'),
(193, 0, '125.39.19.91', '192.168.1.110', 80, 49367, 42, 'Sun Apr 13 13:56:52 2014\n'),
(194, 0, '123.129.232.232', '192.168.1.110', 80, 57777, 42, 'Sun Apr 13 13:56:52 2014\n'),
(195, 0, '123.129.232.231', '192.168.1.110', 80, 49289, 42, 'Sun Apr 13 13:56:52 2014\n'),
(196, 0, '125.39.19.91', '192.168.1.110', 80, 49368, 1772, 'Sun Apr 13 13:56:52 2014\n'),
(197, 0, '125.39.19.91', '192.168.1.110', 80, 49368, 42, 'Sun Apr 13 13:56:52 2014\n'),
(198, 0, '125.39.19.91', '192.168.1.110', 80, 49366, 1772, 'Sun Apr 13 13:56:52 2014\n'),
(199, 0, '125.39.19.91', '192.168.1.110', 80, 49366, 42, 'Sun Apr 13 13:56:52 2014\n'),
(200, 0, '125.39.19.91', '192.168.1.110', 80, 49365, 1772, 'Sun Apr 13 13:56:52 2014\n'),
(201, 0, '125.39.19.91', '192.168.1.110', 80, 49365, 42, 'Sun Apr 13 13:56:52 2014\n'),
(202, 0, '74.125.235.193', '192.168.1.107', 443, 43947, 1662, 'Mon May 12 15:43:33 2014\n'),
(203, 0, '211.151.247.92', '192.168.1.107', 80, 42964, 626, 'Mon May 12 15:43:33 2014\n'),
(204, 0, '211.151.247.92', '192.168.1.107', 80, 42964, 1674, 'Mon May 12 15:43:33 2014\n'),
(205, 0, '211.151.247.92', '192.168.1.107', 80, 42964, 1349, 'Mon May 12 15:43:33 2014\n'),
(206, 0, '211.151.247.92', '192.168.1.107', 80, 42964, 1674, 'Mon May 12 15:43:33 2014\n'),
(207, 0, '211.151.247.38', '192.168.1.107', 80, 53384, 626, 'Mon May 12 15:43:33 2014\n'),
(208, 0, '211.151.247.38', '192.168.1.107', 80, 53384, 86, 'Mon May 12 15:43:33 2014\n'),
(209, 0, '211.151.247.38', '192.168.1.107', 80, 53384, 1674, 'Mon May 12 15:43:33 2014\n'),
(210, 0, '211.151.247.38', '192.168.1.107', 80, 53384, 86, 'Mon May 12 15:43:33 2014\n'),
(211, 0, '211.151.247.38', '192.168.1.107', 80, 53384, 1674, 'Mon May 12 15:43:33 2014\n'),
(212, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 626, 'Mon May 12 15:43:33 2014\n'),
(213, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1315, 'Mon May 12 15:43:33 2014\n'),
(214, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1674, 'Mon May 12 15:43:33 2014\n'),
(215, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1383, 'Mon May 12 15:43:33 2014\n'),
(216, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1685, 'Mon May 12 15:43:33 2014\n'),
(217, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1685, 'Mon May 12 15:43:33 2014\n'),
(218, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1685, 'Mon May 12 15:43:33 2014\n'),
(219, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1685, 'Mon May 12 15:43:33 2014\n'),
(220, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1744, 'Mon May 12 15:43:33 2014\n'),
(221, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1685, 'Mon May 12 15:43:33 2014\n'),
(222, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1685, 'Mon May 12 15:43:33 2014\n'),
(223, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1744, 'Mon May 12 15:43:33 2014\n'),
(224, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1685, 'Mon May 12 15:43:33 2014\n'),
(225, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1744, 'Mon May 12 15:43:33 2014\n'),
(226, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1296, 'Mon May 12 15:43:33 2014\n'),
(227, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1685, 'Mon May 12 15:43:33 2014\n'),
(228, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1075, 'Mon May 12 15:43:33 2014\n'),
(229, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1685, 'Mon May 12 15:43:33 2014\n'),
(230, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1342, 'Mon May 12 15:43:33 2014\n'),
(231, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1342, 'Mon May 12 15:43:33 2014\n'),
(232, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1342, 'Mon May 12 15:43:33 2014\n'),
(233, 1, '192.168.1.1', '192.168.1.107', 53, 42935, 46, 'Mon May 12 15:43:33 2014\n'),
(234, 1, '192.168.1.1', '192.168.1.107', 53, 17418, 46, 'Mon May 12 15:43:33 2014\n'),
(235, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1744, 'Mon May 12 15:43:33 2014\n'),
(236, 1, '192.168.1.1', '192.168.1.107', 53, 40125, 46, 'Mon May 12 15:43:33 2014\n'),
(237, 1, '192.168.1.1', '192.168.1.107', 53, 40125, 46, 'Mon May 12 15:43:33 2014\n'),
(238, 1, '192.168.1.1', '192.168.1.107', 53, 40125, 46, 'Mon May 12 15:43:33 2014\n'),
(239, 1, '192.168.1.1', '192.168.1.107', 53, 40125, 46, 'Mon May 12 15:43:33 2014\n'),
(240, 1, '192.168.1.1', '192.168.1.107', 53, 44685, 46, 'Mon May 12 15:43:33 2014\n'),
(241, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1389, 'Mon May 12 15:43:33 2014\n'),
(242, 1, '192.168.1.1', '192.168.1.107', 53, 56609, 1383, 'Mon May 12 15:43:33 2014\n'),
(243, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1389, 'Mon May 12 15:43:33 2014\n'),
(244, 1, '192.168.1.1', '192.168.1.107', 53, 22279, 46, 'Mon May 12 15:43:33 2014\n'),
(245, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1296, 'Mon May 12 15:43:33 2014\n'),
(246, 1, '192.168.1.1', '192.168.1.107', 53, 5828, 46, 'Mon May 12 15:43:33 2014\n'),
(247, 1, '192.168.1.1', '192.168.1.107', 53, 37045, 46, 'Mon May 12 15:43:33 2014\n'),
(248, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1419, 'Mon May 12 15:43:33 2014\n'),
(249, 1, '192.168.1.1', '192.168.1.107', 53, 37045, 46, 'Mon May 12 15:43:33 2014\n'),
(250, 1, '192.168.1.1', '192.168.1.107', 53, 37045, 46, 'Mon May 12 15:43:33 2014\n'),
(251, 1, '192.168.1.1', '192.168.1.107', 53, 37045, 46, 'Mon May 12 15:43:33 2014\n'),
(252, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 525, 'Mon May 12 15:43:33 2014\n'),
(253, 1, '192.168.1.1', '192.168.1.107', 53, 37045, 46, 'Mon May 12 15:43:33 2014\n'),
(254, 1, '192.168.1.1', '192.168.1.107', 53, 37045, 46, 'Mon May 12 15:43:33 2014\n'),
(255, 1, '192.168.1.1', '192.168.1.107', 53, 59497, 46, 'Mon May 12 15:43:33 2014\n'),
(256, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1744, 'Mon May 12 15:43:33 2014\n'),
(257, 1, '192.168.1.1', '192.168.1.107', 53, 34556, 1383, 'Mon May 12 15:43:33 2014\n'),
(258, 1, '192.168.1.1', '192.168.1.107', 53, 25613, 1383, 'Mon May 12 15:43:33 2014\n'),
(259, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 525, 'Mon May 12 15:43:33 2014\n'),
(260, 1, '192.168.1.1', '192.168.1.107', 53, 50438, 46, 'Mon May 12 15:43:33 2014\n'),
(261, 1, '192.168.1.1', '192.168.1.107', 53, 16561, 1383, 'Mon May 12 15:43:33 2014\n'),
(262, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 784, 'Mon May 12 15:43:33 2014\n'),
(263, 1, '192.168.1.1', '192.168.1.107', 53, 10718, 46, 'Mon May 12 15:43:33 2014\n'),
(264, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 525, 'Mon May 12 15:43:33 2014\n'),
(265, 1, '192.168.1.1', '192.168.1.107', 53, 10718, 46, 'Mon May 12 15:43:33 2014\n'),
(266, 1, '192.168.1.1', '192.168.1.107', 53, 10718, 46, 'Mon May 12 15:43:33 2014\n'),
(267, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1685, 'Mon May 12 15:43:33 2014\n'),
(268, 1, '192.168.1.1', '192.168.1.107', 53, 24955, 1383, 'Mon May 12 15:43:33 2014\n'),
(269, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1419, 'Mon May 12 15:43:33 2014\n'),
(270, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1389, 'Mon May 12 15:43:33 2014\n'),
(271, 0, '112.125.123.23', '192.168.1.107', 80, 40848, 1349, 'Mon May 12 15:43:33 2014\n'),
(272, 0, '112.125.123.23', '192.168.1.107', 80, 40848, 626, 'Mon May 12 15:43:33 2014\n'),
(273, 0, '112.125.123.23', '192.168.1.107', 80, 40848, 2057, 'Mon May 12 15:43:33 2014\n'),
(274, 0, '112.125.123.23', '192.168.1.107', 80, 40848, 1062, 'Mon May 12 15:43:33 2014\n'),
(275, 0, '112.125.123.23', '192.168.1.107', 80, 40848, 1349, 'Mon May 12 15:43:33 2014\n'),
(276, 0, '112.125.123.23', '192.168.1.107', 80, 40848, 790, 'Mon May 12 15:43:33 2014\n'),
(277, 0, '112.125.123.23', '192.168.1.107', 80, 40848, 1419, 'Mon May 12 15:43:33 2014\n'),
(278, 0, '112.125.123.23', '192.168.1.107', 80, 40855, 626, 'Mon May 12 15:43:33 2014\n'),
(279, 0, '112.125.123.23', '192.168.1.107', 80, 40855, 2057, 'Mon May 12 15:43:33 2014\n'),
(280, 0, '112.125.123.23', '192.168.1.107', 80, 40855, 1062, 'Mon May 12 15:43:33 2014\n'),
(281, 0, '112.125.123.23', '192.168.1.107', 80, 40856, 626, 'Mon May 12 15:43:33 2014\n'),
(282, 0, '112.125.123.23', '192.168.1.107', 80, 40856, 2057, 'Mon May 12 15:43:33 2014\n'),
(283, 0, '112.125.123.23', '192.168.1.107', 80, 40856, 1062, 'Mon May 12 15:43:33 2014\n'),
(284, 0, '112.125.123.23', '192.168.1.107', 80, 40856, 1030, 'Mon May 12 15:43:33 2014\n'),
(285, 0, '112.125.123.23', '192.168.1.107', 80, 40856, 517, 'Mon May 12 15:43:33 2014\n'),
(286, 0, '203.208.48.137', '192.168.1.107', 80, 53211, 2057, 'Mon May 12 15:43:33 2014\n'),
(287, 0, '203.208.48.137', '192.168.1.107', 80, 53211, 626, 'Mon May 12 15:43:33 2014\n'),
(288, 0, '67.215.253.139', '192.168.1.107', 80, 56831, 982, 'Mon May 12 15:43:33 2014\n'),
(289, 0, '74.125.235.193', '192.168.1.107', 443, 43947, 1662, 'Mon May 12 17:38:51 2014\n'),
(290, 0, '211.151.247.92', '192.168.1.107', 80, 42964, 626, 'Mon May 12 17:38:51 2014\n'),
(291, 0, '211.151.247.92', '192.168.1.107', 80, 42964, 1674, 'Mon May 12 17:38:51 2014\n'),
(292, 0, '211.151.247.92', '192.168.1.107', 80, 42964, 1349, 'Mon May 12 17:38:51 2014\n'),
(293, 0, '211.151.247.92', '192.168.1.107', 80, 42964, 1674, 'Mon May 12 17:38:51 2014\n'),
(294, 0, '211.151.247.38', '192.168.1.107', 80, 53384, 626, 'Mon May 12 17:38:51 2014\n'),
(295, 0, '211.151.247.38', '192.168.1.107', 80, 53384, 86, 'Mon May 12 17:38:51 2014\n'),
(296, 0, '211.151.247.38', '192.168.1.107', 80, 53384, 1674, 'Mon May 12 17:38:51 2014\n'),
(297, 0, '211.151.247.38', '192.168.1.107', 80, 53384, 86, 'Mon May 12 17:38:51 2014\n'),
(298, 0, '211.151.247.38', '192.168.1.107', 80, 53384, 1674, 'Mon May 12 17:38:51 2014\n'),
(299, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 626, 'Mon May 12 17:38:51 2014\n'),
(300, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1315, 'Mon May 12 17:38:51 2014\n'),
(301, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1674, 'Mon May 12 17:38:51 2014\n'),
(302, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1383, 'Mon May 12 17:38:51 2014\n'),
(303, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1685, 'Mon May 12 17:38:51 2014\n'),
(304, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1685, 'Mon May 12 17:38:51 2014\n'),
(305, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1685, 'Mon May 12 17:38:51 2014\n'),
(306, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1685, 'Mon May 12 17:38:51 2014\n'),
(307, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1744, 'Mon May 12 17:38:51 2014\n'),
(308, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1685, 'Mon May 12 17:38:51 2014\n'),
(309, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1685, 'Mon May 12 17:38:51 2014\n'),
(310, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1744, 'Mon May 12 17:38:51 2014\n'),
(311, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1685, 'Mon May 12 17:38:51 2014\n'),
(312, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1744, 'Mon May 12 17:38:51 2014\n'),
(313, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1296, 'Mon May 12 17:38:51 2014\n'),
(314, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1685, 'Mon May 12 17:38:51 2014\n'),
(315, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1075, 'Mon May 12 17:38:51 2014\n'),
(316, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1685, 'Mon May 12 17:38:51 2014\n'),
(317, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1342, 'Mon May 12 17:38:51 2014\n'),
(318, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1342, 'Mon May 12 17:38:51 2014\n'),
(319, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1342, 'Mon May 12 17:38:51 2014\n'),
(320, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1744, 'Mon May 12 17:38:51 2014\n'),
(321, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1389, 'Mon May 12 17:38:51 2014\n'),
(322, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1389, 'Mon May 12 17:38:51 2014\n'),
(323, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1296, 'Mon May 12 17:38:51 2014\n'),
(324, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1419, 'Mon May 12 17:38:51 2014\n'),
(325, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 525, 'Mon May 12 17:38:51 2014\n'),
(326, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1744, 'Mon May 12 17:38:51 2014\n'),
(327, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 525, 'Mon May 12 17:38:51 2014\n'),
(328, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 784, 'Mon May 12 17:38:51 2014\n'),
(329, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 525, 'Mon May 12 17:38:51 2014\n'),
(330, 1, '192.168.1.1', '192.168.1.107', 53, 42935, 46, 'Mon May 12 17:38:51 2014\n'),
(331, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1685, 'Mon May 12 17:38:51 2014\n'),
(332, 1, '192.168.1.1', '192.168.1.107', 53, 17418, 46, 'Mon May 12 17:38:51 2014\n'),
(333, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1419, 'Mon May 12 17:38:51 2014\n'),
(334, 1, '192.168.1.1', '192.168.1.107', 53, 40125, 46, 'Mon May 12 17:38:51 2014\n'),
(335, 0, '119.167.209.17', '192.168.1.107', 80, 52503, 1389, 'Mon May 12 17:38:51 2014\n'),
(336, 1, '192.168.1.1', '192.168.1.107', 53, 40125, 46, 'Mon May 12 17:38:51 2014\n'),
(337, 1, '192.168.1.1', '192.168.1.107', 53, 40125, 46, 'Mon May 12 17:38:51 2014\n'),
(338, 1, '192.168.1.1', '192.168.1.107', 53, 40125, 46, 'Mon May 12 17:38:51 2014\n'),
(339, 1, '192.168.1.1', '192.168.1.107', 53, 44685, 46, 'Mon May 12 17:38:51 2014\n'),
(340, 1, '192.168.1.1', '192.168.1.107', 53, 56609, 1383, 'Mon May 12 17:38:51 2014\n'),
(341, 0, '112.125.123.23', '192.168.1.107', 80, 40848, 1349, 'Mon May 12 17:38:51 2014\n'),
(342, 1, '192.168.1.1', '192.168.1.107', 53, 22279, 46, 'Mon May 12 17:38:51 2014\n'),
(343, 0, '112.125.123.23', '192.168.1.107', 80, 40848, 626, 'Mon May 12 17:38:51 2014\n'),
(344, 1, '192.168.1.1', '192.168.1.107', 53, 5828, 46, 'Mon May 12 17:38:51 2014\n'),
(345, 0, '112.125.123.23', '192.168.1.107', 80, 40848, 2057, 'Mon May 12 17:38:51 2014\n'),
(346, 1, '192.168.1.1', '192.168.1.107', 53, 37045, 46, 'Mon May 12 17:38:51 2014\n'),
(347, 1, '192.168.1.1', '192.168.1.107', 53, 37045, 46, 'Mon May 12 17:38:51 2014\n'),
(348, 0, '112.125.123.23', '192.168.1.107', 80, 40848, 1062, 'Mon May 12 17:38:51 2014\n'),
(349, 0, '112.125.123.23', '192.168.1.107', 80, 40848, 1349, 'Mon May 12 17:38:51 2014\n'),
(350, 1, '192.168.1.1', '192.168.1.107', 53, 37045, 46, 'Mon May 12 17:38:51 2014\n'),
(351, 1, '192.168.1.1', '192.168.1.107', 53, 37045, 46, 'Mon May 12 17:38:51 2014\n'),
(352, 0, '112.125.123.23', '192.168.1.107', 80, 40848, 790, 'Mon May 12 17:38:51 2014\n'),
(353, 1, '192.168.1.1', '192.168.1.107', 53, 37045, 46, 'Mon May 12 17:38:51 2014\n'),
(354, 0, '112.125.123.23', '192.168.1.107', 80, 40848, 1419, 'Mon May 12 17:38:51 2014\n'),
(355, 1, '192.168.1.1', '192.168.1.107', 53, 37045, 46, 'Mon May 12 17:38:51 2014\n'),
(356, 1, '192.168.1.1', '192.168.1.107', 53, 59497, 46, 'Mon May 12 17:38:51 2014\n'),
(357, 0, '112.125.123.23', '192.168.1.107', 80, 40855, 626, 'Mon May 12 17:38:51 2014\n'),
(358, 0, '112.125.123.23', '192.168.1.107', 80, 40855, 2057, 'Mon May 12 17:38:51 2014\n'),
(359, 1, '192.168.1.1', '192.168.1.107', 53, 34556, 1383, 'Mon May 12 17:38:51 2014\n'),
(360, 0, '112.125.123.23', '192.168.1.107', 80, 40855, 1062, 'Mon May 12 17:38:51 2014\n'),
(361, 1, '192.168.1.1', '192.168.1.107', 53, 25613, 1383, 'Mon May 12 17:38:51 2014\n'),
(362, 0, '112.125.123.23', '192.168.1.107', 80, 40856, 626, 'Mon May 12 17:38:51 2014\n'),
(363, 1, '192.168.1.1', '192.168.1.107', 53, 50438, 46, 'Mon May 12 17:38:51 2014\n'),
(364, 1, '192.168.1.1', '192.168.1.107', 53, 16561, 1383, 'Mon May 12 17:38:51 2014\n'),
(365, 0, '112.125.123.23', '192.168.1.107', 80, 40856, 2057, 'Mon May 12 17:38:51 2014\n'),
(366, 0, '112.125.123.23', '192.168.1.107', 80, 40856, 1062, 'Mon May 12 17:38:51 2014\n'),
(367, 1, '192.168.1.1', '192.168.1.107', 53, 10718, 46, 'Mon May 12 17:38:51 2014\n'),
(368, 0, '112.125.123.23', '192.168.1.107', 80, 40856, 1030, 'Mon May 12 17:38:51 2014\n'),
(369, 1, '192.168.1.1', '192.168.1.107', 53, 10718, 46, 'Mon May 12 17:38:51 2014\n'),
(370, 0, '112.125.123.23', '192.168.1.107', 80, 40856, 517, 'Mon May 12 17:38:51 2014\n'),
(371, 1, '192.168.1.1', '192.168.1.107', 53, 10718, 46, 'Mon May 12 17:38:51 2014\n'),
(372, 1, '192.168.1.1', '192.168.1.107', 53, 24955, 1383, 'Mon May 12 17:38:51 2014\n'),
(373, 0, '203.208.48.137', '192.168.1.107', 80, 53211, 2057, 'Mon May 12 17:38:51 2014\n'),
(374, 0, '203.208.48.137', '192.168.1.107', 80, 53211, 626, 'Mon May 12 17:38:51 2014\n'),
(375, 0, '67.215.253.139', '192.168.1.107', 80, 56831, 982, 'Mon May 12 17:38:51 2014\n');

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
