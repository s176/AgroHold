#ifndef _AgroHold_PanProject_h_
#define _AgroHold_PanProject_h_

class PanProject : public WithProjectLayout<ParentCtrl> 
{
	Sql sql;

	void NameUpdate()
	{
		try
		{
			sql.Execute("SELECT * FROM Project WHERE Param = 'Name'");
			bool inbase = sql.Fetch();
			if (inbase)
			{
				sql.Execute("UPDATE Project SET strVal = '" + AsString(edName.GetData()) + "' WHERE Param = 'Name'");
			}
			else
			{
				sql.Execute("INSERT INTO Project (Param, strVal) VALUES ('Name', '" + AsString(edName.GetData()) + "')");
			}
				
		}
		catch(SqlExc &e)
		{
			Exclamation("[* " + DeQtfLf(e) + "]");
		}
	}

	void YearUpdate()
	{
		try
		{
			sql.Execute("SELECT * FROM Project WHERE Param = 'Year'");
			bool inbase = sql.Fetch();
			if (inbase)
			{
				sql.Execute("UPDATE Project SET intVal = '" + AsString(edYear.GetData()) + "' WHERE Param = 'Year'");
			}
			else
			{
				sql.Execute("INSERT INTO Project (Param, intVal) VALUES ('Year', '" + AsString(edYear.GetData()) + "')");
			}
				
		}
		catch(SqlExc &e)
		{
			Exclamation("[* " + DeQtfLf(e) + "]");
		}
	}

	void HorizontUpdate()
	{
		int h = edHorizont.GetData();
		if (h < 1 || h > 10)
		{
			PromptOK("Горизонт планування в межах 1 - 10 років");
			edHorizont.SetData(3);
			return;
		}
		try
		{
			sql.Execute("SELECT * FROM Project WHERE Param = 'Horizont'");
			bool inbase = sql.Fetch();
			if (inbase)
			{
				sql.Execute("UPDATE Project SET intVal = '" + AsString(edHorizont.GetData()) + "' WHERE Param = 'Horizont'");
			}
			else
			{
				sql.Execute("INSERT INTO Project (Param, intVal) VALUES ('Horizont', '" + AsString(edHorizont.GetData()) + "')");
			}
				
		}
		catch(SqlExc &e)
		{
			Exclamation("[* " + DeQtfLf(e) + "]");
		}
	}

public:        	
	
	void Populate()
	{
		sql.Execute("SELECT * FROM Project WHERE Param = 'Name'");
		if(sql.Fetch())
		{
			edName.SetData(AsString(sql[2]));
		}	
		sql.Execute("SELECT * FROM Project WHERE Param = 'Year'");
		if(sql.Fetch())
		{
			edYear.SetData(AsString(sql[1]));
		}
		else
		{
			Date date = GetSysDate();
			sql.Execute("INSERT INTO Project (Param, intVal) VALUES ('Year', '" + AsString((int)date.year) + "')");
			edYear.SetData((int)date.year);
		}
		sql.Execute("SELECT * FROM Project WHERE Param = 'Horizont'");
		if(sql.Fetch())
		{
			edHorizont.SetData(AsString(sql[1]));
		}
		else
		{
			sql.Execute("INSERT INTO Project (Param, intVal) VALUES ('Horizont', '5')");
			edHorizont.SetData(5);
		}
	}

    typedef PanProject CLASSNAME;
 	PanProject()
 	{
 		CtrlLayout(*this);
 		SetFrame(InsetFrame());
 		edName.WhenAction = THISBACK(NameUpdate);
 		edYear.WhenAction = THISBACK(YearUpdate);
 		edHorizont.WhenAction = THISBACK(HorizontUpdate);
	};
};


#endif
