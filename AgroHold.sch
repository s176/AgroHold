TABLE(Project)
	STRING (Param, 200)
	INT (intVal)
	STRING (strVal, 200)
END_TABLE

TABLE(Cultures)
	INT (ID)
	STRING (Name, 200)
	INT (Type)
	INT (Rotation)
END_TABLE

TABLE(CultureType)
	INT (ID)
	STRING (Name, 200)
	INT (Min)
	INT (Max)
END_TABLE

TABLE(Fields)
	INT (ID)
	STRING (Name, 200)
	DOUBLE (Area)
	INT (Culture)		// Поточна культура на полі
	INT (Year1)			// Технологія у перший рік
	INT (Year2)			// Технологія у другий рік
	INT (Year3)
	INT (Year4)
	INT (Year5)
	INT (Year6)
	INT (Year7)
	INT (Year8)
	INT (Year9)
	INT (Year10)
END_TABLE

TABLE(FieldsBonitet)
	INT (ID)
	INT (Field)
	INT (CultureType)
	INT (Bal)
END_TABLE

TABLE(Poperedniki)
	INT (ID)
	INT (Id_pop)
	INT (Id_nast)
	DOUBLE (Vpliv)
END_TABLE

TABLE(Energo)
	INT (ID)
	STRING (Name, 200)
	DOUBLE (Balance)
	DOUBLE (NormYear)
	DOUBLE (NormAmort)
	DOUBLE (NormTor)
END_TABLE

TABLE(Machinery)
	INT (ID)
	STRING (Name, 200)
	DOUBLE (Balance)
	DOUBLE (NormYear)
	DOUBLE (NormAmort)
	DOUBLE (NormTor)
END_TABLE

TABLE(MatType)
	INT (ID)
	STRING (Name, 200)
END_TABLE

TABLE(Materials)
	INT (ID)
	STRING (Name, 200)
	INT (Type)
	STRING (Odvim, 200)
	DOUBLE (Price)
END_TABLE

TABLE(Operations)
	INT (ID)
	STRING (Name, 200)
	STRING (Odvim, 200)
	INT (Duration)
END_TABLE

TABLE(MTA)
	INT (ID)
	INT (ID_operation)
	INT (ID_Tractor)
	INT (ID_Machinery)
	DOUBLE (Norm_daily)
	DOUBLE (OP)
	DOUBLE (Palne)
	INT (Culture)	
END_TABLE

TABLE(Technologies)
	INT (ID)
	INT (Culture)
	DOUBLE (Urogai)
	DOUBLE (Cina)
	DOUBLE (Cost)
	STRING (Comment, 250)	
END_TABLE

TABLE(Processes)
	INT (ID)
	INT (ID_tech)
	INT (Month)
	INT (ID_operation)
	DOUBLE (Obsyag)
END_TABLE

TABLE(MaterForTech)
	INT (ID)
	INT (ID_tech)
	INT (Month)
	INT (ID_material)
	DOUBLE (Norm)
END_TABLE

TABLE(Graphik)
	INT (ID)
	INT (Year)
	INT (Technology)
	STRING (Date, 200)
	INT(Process)
	INT (MTA)
	INT (Duration)
END_TABLE