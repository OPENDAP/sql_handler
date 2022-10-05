
/* Use this to populate the test database with data */

USE sqlite_test;
CREATE TABLE sqlh_table 
(a integer NOT NULL DEFAULT 0,
b real NOT NULL DEFAULT 0,
c varchar(50),
CONSTRAINT pk PRIMARY KEY (a));

INSERT INTO sqlh_table(a, b, c) VALUES ('1', '81.0', 'string_a'), ('2', '61.1', 'string_b'), ('3', '51.0', 'string_c'), ('4', '2100.0', 'string_d'), ('5', '21.0', 'string_e'), ('6', '4133.0', 'string_f'), ('7', '31.4', 'string_g'), ('8', '21.3', 'string_h'), ('9', '11.6', 'string_i'), ('10', '22.2', 'string_j');

