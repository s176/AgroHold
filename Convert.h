#ifndef _AgroHold_Convert_h_
#define _AgroHold_Convert_h_

//Конвертація (виведення) дійсного числа у потрібному числовому ###.##-форматі
struct ConvDouble : Convert 
{
	Value Format(const Value &q) const
	{
		return q.IsNull() ? Null : UPP::Format("%.2f", q);
	}
};

//Конвертація ID МТА у їх назви
struct MTAConvert : Convert 
{
		virtual Value Format(const Value& q) const;
};

// Конвертація даних МТА для Гриду
Value MTAConvert::Format(const Value& q) const
{
	if(q.IsNull()) return Null;
	static VectorMap<int, String> data;
	String text;
	static Time lastcleartime;
	if(GetSysTime() - lastcleartime > 5)
	{
		data.Clear();
		lastcleartime=GetSysTime();
	}
	if(data.Find(q) > 0)
		return data.Get(q, String());
	else 
	{
		Sql sql;
		sql.Execute("SELECT MTA.ID, Energo.Name, Machinery.Name FROM MTA, Energo, Machinery Where MTA.ID_Tractor = Energo.ID AND MTA.ID_Machinery = Machinery.ID AND MTA.ID = " + AsString(q));
		if(sql.Fetch()) 
		{
			int sid = sql[0];
			text=sql[1]; 
			text<<" & ";
			text<<sql[2];
			data.Add(sid, text);
			return data.Get(q, String());
		}
		else
			return (String)("");
	}
}


//Конвертація ID типу культур у їх назви
struct TypeConvert : Convert 
{
		virtual Value Format(const Value& q) const;
};

// Конвертація даних типу культур для Гриду
Value TypeConvert::Format(const Value& q) const
{
	if(q.IsNull()) return Null;
	static VectorMap<int, String> data;
	String text;
	static Time lastcleartime;
	if(GetSysTime() - lastcleartime > 5)
	{
		data.Clear();
		lastcleartime=GetSysTime();
	}
	if(data.Find(q) > 0)
		return data.Get(q, String());
	else 
	{
		Sql sql;
		sql.Execute("SELECT * FROM CultureType Where ID = " + AsString(q));
		if(sql.Fetch()) 
		{
			int sid = sql[0];
			text=sql[1]; 
			data.Add(sid, text);
			return data.Get(q, String());
		}
		else
			return (String)("");
	}
}

//Конвертація ID культур у їх назви
struct CultConvert : Convert 
{
		virtual Value Format(const Value& q) const;
};

// Конвертація даних  культур для Гриду
Value CultConvert::Format(const Value& q) const
{
	if(q.IsNull()) return Null;
	static VectorMap<int, String> data;
	String text;
	static Time lastcleartime;
	if(GetSysTime() - lastcleartime > 5)
	{
		data.Clear();
		lastcleartime=GetSysTime();
	}
	if(data.Find(q) > 0)
		return data.Get(q, String());
	else 
	{
		Sql sql;
		sql.Execute("SELECT * FROM Cultures Where ID = " + AsString(q));
		if(sql.Fetch()) 
		{
			int sid = sql[0];
			text=sql[1]; 
			data.Add(sid, text);
			return data.Get(q, String());
		}
		else
			return (String)("");
	}
}

//Конвертація ID типу матеріалу у їх назви
struct MatTypeConvert : Convert 
{
		virtual Value Format(const Value& q) const;
};

// Конвертація даних типу матеріалу для Гриду
Value MatTypeConvert::Format(const Value& q) const
{
	if(q.IsNull()) return Null;
	static VectorMap<int, String> data;
	String text;
	static Time lastcleartime;
	if(GetSysTime() - lastcleartime > 5)
	{
		data.Clear();
		lastcleartime=GetSysTime();
	}
	if(data.Find(q) > 0)
		return data.Get(q, String());
	else 
	{
		Sql sql;
		sql.Execute("SELECT * FROM MatType Where ID = " + AsString(q));
		if(sql.Fetch()) 
		{
			int sid = sql[0];
			text=sql[1]; 
			data.Add(sid, text);
			return data.Get(q, String());
		}
		else
			return (String)("");
	}
}

//Конвертація ID енергомашини у їх назви
struct EnergoConvert : Convert 
{
		virtual Value Format(const Value& q) const;
};

// Конвертація даних енергомашини для Гриду
Value EnergoConvert::Format(const Value& q) const
{
	if(q.IsNull()) return Null;
	static VectorMap<int, String> data;
	String text;
	static Time lastcleartime;
	if(GetSysTime() - lastcleartime > 5)
	{
		data.Clear();
		lastcleartime=GetSysTime();
	}
	if(data.Find(q) > 0)
		return data.Get(q, String());
	else 
	{
		Sql sql;
		sql.Execute("SELECT * FROM Energo Where ID = " + AsString(q));
		if(sql.Fetch()) 
		{
			int sid = sql[0];
			text=sql[1]; 
			data.Add(sid, text);
			return data.Get(q, String());
		}
		else
			return (String)("");
	}
}

//Конвертація ID с.г. машини у їх назви
struct MashConvert : Convert 
{
		virtual Value Format(const Value& q) const;
};

// Конвертація даних с.г. машини для Гриду
Value MashConvert::Format(const Value& q) const
{
	if(q.IsNull()) return Null;
	static VectorMap<int, String> data;
	String text;
	static Time lastcleartime;
	if(GetSysTime() - lastcleartime > 5)
	{
		data.Clear();
		lastcleartime=GetSysTime();
	}
	if(data.Find(q) > 0)
		return data.Get(q, String());
	else 
	{
		Sql sql;
		sql.Execute("SELECT * FROM Machinery Where ID = " + AsString(q));
		if(sql.Fetch()) 
		{
			int sid = sql[0];
			text=sql[1]; 
			data.Add(sid, text);
			return data.Get(q, String());
		}
		else
			return (String)("");
	}
}

//Конвертація ID операцій у їх назви
struct OperationConvert : Convert 
{
		virtual Value Format(const Value& q) const;
};

// Конвертація даних операцій для Гриду
Value OperationConvert::Format(const Value& q) const
{
	if(q.IsNull()) return Null;
	static VectorMap<int, String> data;
	String text;
	static Time lastcleartime;
	if(GetSysTime() - lastcleartime > 5)
	{
		data.Clear();
		lastcleartime=GetSysTime();
	}
	if(data.Find(q) > 0)
		return data.Get(q, String());
	else 
	{
		Sql sql;
		sql.Execute("SELECT * FROM Operations Where ID = " + AsString(q));
		if(sql.Fetch()) 
		{
			int sid = sql[0];
			text=sql[1]; 
			text<<", ";
			text<<sql[2];
			data.Add(sid, text);
			return data.Get(q, String());
		}
		else
			return (String)("");
	}
}

//Конвертація ID матеріалів у їх назви
struct MaterConvert : Convert 
{
		virtual Value Format(const Value& q) const;
};

// Конвертація даних матеріалів для Гриду
Value MaterConvert::Format(const Value& q) const
{
	if(q.IsNull()) return Null;
	static VectorMap<int, String> data;
	String text;
	static Time lastcleartime;
	if(GetSysTime() - lastcleartime > 5)
	{
		data.Clear();
		lastcleartime=GetSysTime();
	}
	if(data.Find(q) > 0)
		return data.Get(q, String());
	else 
	{
		Sql sql;
		sql.Execute("SELECT * FROM Materials Where ID = " + AsString(q));
		if(sql.Fetch()) 
		{
			int sid = sql[0];
			text=sql[1]; 
			text<<", ";
			text<<sql[2];
			data.Add(sid, text);
			return data.Get(q, String());
		}
		else
			return (String)("");
	}
}

//Конвертація ID місяців у їх назви
struct MonthConvert : Convert 
{
		virtual Value Format(const Value& q) const;
};

// Конвертація даних місяців для Гриду
Value MonthConvert::Format(const Value& q) const
{
	if(q.IsNull()) return Null;
	static VectorMap<int, String> data;
	//data.Clear();
	int sid;
	String text;
	sid = 0; text = ""; data.Add(sid, text);
	sid = 1; text = "підготовка: липень"; data.Add(sid, text);
	sid = 2; text = "підготовка: серпень"; data.Add(sid, text);
	sid = 3; text = "підготовка: вересень"; data.Add(sid, text);
	sid = 4; text = "підготовка: жовтень"; data.Add(sid, text);
	sid = 5; text = "січень"; data.Add(sid, text);
	sid = 6; text = "лютий"; data.Add(sid, text);
	sid = 7; text = "березень"; data.Add(sid, text);
	sid = 8; text = "квітень"; data.Add(sid, text);
	sid = 9; text = "травень"; data.Add(sid, text);
	sid = 10; text = "червень"; data.Add(sid, text);
	sid = 11; text = "липень"; data.Add(sid, text);
	sid = 12; text = "серпень"; data.Add(sid, text);
	sid = 13; text = "вересень"; data.Add(sid, text);
	sid = 14; text = "жовтень"; data.Add(sid, text);
	sid = 15; text = "листопад"; data.Add(sid, text);
	sid = 16; text = "грудень"; data.Add(sid, text);
	if(data.Find(q) > 0)
		return data.Get(q, String());
	else 
		return (String)("");
}

//Конвертація ID технології у назви культур в технології для гриду сівозмін
struct TechCultConvert : Convert 
{
		virtual Value Format(const Value& q) const;
};

// Конвертація даних культур для гриду сівозмін
Value TechCultConvert::Format(const Value& q) const
{
if(q.IsNull()) return Null;
	static VectorMap<int, String> data;
	String text;
	static Time lastcleartime;
	if(GetSysTime() - lastcleartime > 5)
	{
		data.Clear();
		lastcleartime=GetSysTime();
	}
	if(data.Find(q) > 0)
		return data.Get(q, String());
	else 
	{
		Sql sql;
		sql.Execute("SELECT Technologies.ID, Cultures.Name, Technologies.Urogai FROM Cultures INNER JOIN Technologies ON Cultures.ID = Technologies.Culture Where Technologies.ID = " + AsString(q));
		if(sql.Fetch()) 
		{
			int sid = sql[0];
			text=sql[1]; 
			text<<", ";
			text<<sql[2];
			text<<" т/га";
			data.Add(sid, text);
			return data.Get(q, String());
		}
		else
			return (String)("");
	}
}

#endif
