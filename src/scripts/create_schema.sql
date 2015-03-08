/*
#######################################################################################################################
# Copyright (C) 2015  Lukas Georgieff
# Last modified: 06/03/2015
# Description: Creates the (postgresql) SQL data base schema for the server side of translate.
#              Usage: psql --username translate --dbname translate --file create_schema.sql
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

/*
#######################################################################################################################
### Create all tables
#######################################################################################################################
*/
CREATE TABLE language
(
id char(2) NOT NULL PRIMARY KEY,
name varchar(64) NOT NULL UNIQUE,
CHECK (char_length(trim(id)) = 2),
CHECK (char_length(trim(name)) > 0)
);

CREATE TABLE comment
(
id serial NOT NULL PRIMARY KEY,
comment varchar(256) NOT NULL UNIQUE,
CHECK (char_length(trim(comment)) > 0)
);

CREATE TABLE abbreviation
(
id serial NOT NULL PRIMARY KEY,	
abbreviation varchar(16) NOT NULL UNIQUE,
CHECK (char_length(trim(abbreviation)) > 0)
);

CREATE TABLE word_class
(
id varchar(8) NOT NULL PRIMARY KEY,
name varchar(64) NOT NULL UNIQUE,
CHECK (char_length(trim(id)) > 0),
CHECK (char_length(trim(name)) > 0)
);

CREATE TABLE gender
(
id char(1) NOT NULL PRIMARY KEY,
description varchar(64) NOT NULL
);

CREATE TYPE numerus AS ENUM ('pl.', 'sg.');

CREATE TABLE phrase(
id serial NOT NULL PRIMARY KEY,
phrase varchar(128) NOT NULL,
language char(2) REFERENCES language(id) NOT NULL,
word_class varchar(64) REFERENCES word_class(id),
gender char(1) REFERENCES gender(id),
numerus numerus,
CHECK (char_length(trim(phrase)) > 0),
unique(phrase, language, word_class, gender, numerus)
);

CREATE TABLE phrase_comment
(
phrase_id integer REFERENCES phrase(id) NOT NULL,
comment_id integer REFERENCES comment(id) NOT NULL,
unique(phrase_id, comment_id)
);

CREATE TABLE phrase_abbreviation
(
phrase_id integer REFERENCES phrase(id) NOT NULL,
abbreviation_id integer REFERENCES comment(id) NOT NULL,
unique(phrase_id, abbreviation_id)
);

CREATE TABLE phrase_translation
(
phrase_id_in integer REFERENCES phrase(id) NOT NULL,
phrase_id_out integer REFERENCES phrase(id) NOT NULL,
unique(phrase_id_in, phrase_id_out)
);

/*
#######################################################################################################################
### Insert the basic values into all tables
#######################################################################################################################
*/
INSERT INTO word_class VALUES ('adj', 'adjective');
INSERT INTO word_class VALUES ('adv', 'adverb/adverbial');
INSERT INTO word_class VALUES ('past-p', 'past participle');
INSERT INTO word_class VALUES ('verb', 'verb (infinitive)');
INSERT INTO word_class VALUES ('pres-p', 'present participle');
INSERT INTO word_class VALUES ('prep', 'preposition/adposition');
INSERT INTO word_class VALUES ('conj', 'conjunction');
INSERT INTO word_class VALUES ('pron', 'pronoun');
INSERT INTO word_class VALUES ('prefix', 'prefix');
INSERT INTO word_class VALUES ('suffix', 'suffix');
INSERT INTO word_class VALUES ('noun', 'noun');

INSERT INTO gender VALUES ('m', 'der - männlich (Maskulinum)');
INSERT INTO gender VALUES ('f', 'die - weiblich (Feminimum)');
INSERT INTO gender VALUES ('n', 'das sachlich (Neutrum)');

INSERT INTO language VALUES ('BG', 'български');
INSERT INTO language VALUES ('BS', 'bosanski');
INSERT INTO language VALUES ('CS', 'český');
INSERT INTO language VALUES ('DA', 'dansk');
INSERT INTO language VALUES ('DE', 'Deutsch');
INSERT INTO language VALUES ('EL', 'ελληνικά');
INSERT INTO language VALUES ('EN', 'English');
INSERT INTO language VALUES ('EO', 'Esperanto');
INSERT INTO language VALUES ('ES', 'español');
INSERT INTO language VALUES ('FI', 'suomi');
INSERT INTO language VALUES ('FR', 'français');
INSERT INTO language VALUES ('HR', 'hrvatski');
INSERT INTO language VALUES ('HU', 'magyar');
INSERT INTO language VALUES ('IS', 'íslenska');
INSERT INTO language VALUES ('IT', 'italiano');
INSERT INTO language VALUES ('LA', 'Latinum');
INSERT INTO language VALUES ('NL', 'Nederlands');
INSERT INTO language VALUES ('NO', 'norsk');
INSERT INTO language VALUES ('PL', 'polski');
INSERT INTO language VALUES ('PT', 'português');
INSERT INTO language VALUES ('RO', 'română');
INSERT INTO language VALUES ('RU', 'русский язык');
INSERT INTO language VALUES ('SK', 'slovenčina');
INSERT INTO language VALUES ('SQ', 'shqipe');
INSERT INTO language VALUES ('SR', 'српски');
INSERT INTO language VALUES ('SV', 'svenska');
INSERT INTO language VALUES ('TR', 'Türkçe');
