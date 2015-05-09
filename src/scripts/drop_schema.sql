/*
#######################################################################################################################
# Copyright (C) 2015  Lukas Georgieff
# Last modified: 04/04/2015
# Description: Drops the (postgresql) SQL data base schema for the server side of translate.
#              Usage: psql --username translate --dbname translate --file drop_schema.sql
#######################################################################################################################
*/


/*
#######################################################################################################################
# This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public
# License as published by the Free Software Foundation in version 2.
#
# This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
# warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with this program; if not, write to the Free
# Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#######################################################################################################################
*/

DROP TABLE phrase_translation CASCADE;
DROP TABLE phrase_comment CASCADE;
DROP TABLE phrase CASCADE;
DROP TYPE numerus CASCADE;
DROP TABLE language CASCADE;
DROP TABLE word_class_description CASCADE;
DROP TABLE comment CASCADE;
DROP TABLE abbreviation CASCADE;
DROP TABLE phrase_abbreviation CASCADE;
DROP TABLE gender_description CASCADE;
