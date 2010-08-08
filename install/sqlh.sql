CREATE TABLE "sqlh_table"(
  "int" integer NOT NULL DEFAULT 0,
  "real" real NOT NULL DEFAULT 0,
  "varchar" character varying(50),
  CONSTRAINT pk PRIMARY KEY ("int")
);
ALTER TABLE sqlh_table OWNER TO postgres;
COMMENT ON COLUMN sqlh_table."int" IS 'comment';
COMMENT ON COLUMN sqlh_table."real" IS 'comment ''real''
';

INSERT INTO sqlh_table("int", "real", "varchar") VALUES 
	('1', '81.0', 'string_a'),
	('2', '61.1', 'string_b'),
	('3', '51.0', 'string_c'),
	('4', '2100.0', 'string_d'),
	('5', '21.0', 'string_e'),
	('6', '4133.0', 'string_f'),
	('7', '31.4', 'string_g'),
	('8', '21.3', 'string_h'),
	('9', '11.6', 'string_i'),
	('10', '22.2', 'string_j')
    ;
\commit
\quit
