-- phpMyAdmin SQL Dump
-- version 4.0.6
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: May 21, 2014 at 01:01 PM
-- Server version: 5.5.33
-- PHP Version: 5.5.3

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `Aquarium`
--

-- --------------------------------------------------------

--
-- Table structure for table `Aquarium`
--

CREATE TABLE `Aquarium` (
  `id acq` int(11) NOT NULL AUTO_INCREMENT,
  `name` text NOT NULL,
  PRIMARY KEY (`id acq`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=43 ;



--
-- Table structure for table `Sensor`
--

CREATE TABLE `Sensor` (
  `id sens` int(11) NOT NULL AUTO_INCREMENT,
  `name` text NOT NULL,
  PRIMARY KEY (`id sens`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=43 ;


--
-- Table structure for table `Users`
--

CREATE TABLE `Users` (
  `id user` int(11) NOT NULL AUTO_INCREMENT,
  `name` text NOT NULL,
  `password` text NOT NULL,
  PRIMARY KEY (`id user`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=43 ;


--
-- Table structure for table `Device`
--

CREATE TABLE `Device` (
  `id dev` int(11) NOT NULL AUTO_INCREMENT,
  `name` text NOT NULL,
  `status` text NOT NULL,
  PRIMARY KEY (`id dev`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=43 ;



--
-- Table structure for table `Lecture`
--

CREATE TABLE `Lecture` (
  `id acq` int(11) NOT NULL AUTO_INCREMENT,
  'id sens' int(11) NOT NULL AUTO_INCREMENT,
  `data` int NOT NULL,
  `value` float NOT NULL,
  PRIMARY KEY ('id acq', 'id sens')
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=43 ;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;

