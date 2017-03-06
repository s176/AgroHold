#ifndef _AgroHold_PanPosiv_h_
#define _AgroHold_PanPosiv_h_

class PanPosiv : public WithPanelLayout<ParentCtrl> 
{	
	int Horizont; // горизонт планування - скільки років плануємо
	GridCtrl grid;
	Sql sql;
	Id ID;
	Splitter split;
	String TableName;
	Switch radio1, radio2;
	WithPanelLayout<ParentCtrl> sidectrl;
	
	void Radio1()
	{
		Populate();
	}
	
	void Radio2()
	{
		Populate();
	}
	
public:        	
	
	void Populate()
	{
		// установим горизонт планирования
		sql.Execute("SELECT * FROM Project WHERE Param = 'Horizont'");
		if(sql.Fetch())
			Horizont = sql[1];	
		// удаление имеющихся и вывод нужных стобцов
		grid.Clear();
		grid.RemoveColumn(0, grid.GetColumnCount()); // удаляем все имеющиеся стобцы
		grid.AddIndex(ID);
 		grid.AddColumn("Показник", 150);
 		grid.AddColumn("Поточний рік", 100).AlignRight().SetConvert(Single<ConvDouble>()).DoSum();
		for (int i = 0; i < Horizont; i++)
			grid.AddColumn("", 100).AlignRight().SetConvert(Single<ConvDouble>()).DoSum();
		// установим заголовки лет в таблице
		int cyear;
		sql.Execute("SELECT * FROM Project WHERE Param = 'Year'");
		if(sql.Fetch())
			cyear = sql[1];
		for (int i = 0; i <= Horizont; i++)
			grid.GetColumn(2 + i).Name(AsString(cyear + i));
		grid.RefreshTop();
		
		double totalarea, tempfield;
		int row = 0;
		String text, sqlrequest, Year;
		sql.Clear();
		
		if (radio1.GetData() == 0)
		{
			if (radio2.GetData() == 0)
			{
				totalarea = 100;
			}
			if (radio2.GetData() == 1)
			{
				sql.Execute("SELECT Sum(Fields.Area) FROM Fields");
				if(sql.Fetch())
				totalarea = sql[0];
			}
			sql.Execute("SELECT * FROM Cultures ORDER BY Name");
			while(sql.Fetch())
			{
				grid.Add(sql[0]);
				grid.Set(row, 1, sql[1]);
				row++;
			}
			
			for (int row = 0; row < grid.GetCount(); row++)
			{
				
				sqlrequest = "SELECT Cultures.ID, Sum(Fields.Area) FROM Cultures LEFT JOIN Fields ON Cultures.ID = Fields.Culture WHERE Cultures.ID = ";
				sqlrequest += AsString(grid.Get(row, 0));
				sqlrequest += " GROUP BY Cultures.ID";
				sql.Execute(sqlrequest);
				if(sql.Fetch())
					tempfield = sql[1];
				if (IsNull(sql[1]))
					tempfield = 0;
				grid.Set(row, 2, 100 * tempfield / totalarea);
				
				for (int year = 1; year <= Horizont; year++)
				{ 
					Year = AsString(year);
					sqlrequest = "SELECT Cultures.ID, Cultures.Name, Sum(Fields.Area) FROM (Cultures LEFT JOIN Technologies ON Cultures.ID = Technologies.Culture) LEFT JOIN Fields ON Technologies.ID = Fields.Year" + Year + " WHERE Cultures.ID = ";
					sqlrequest += AsString(grid.Get(row, 0));
					sqlrequest += " GROUP BY Cultures.ID";
					sql.Execute(sqlrequest);
					if(sql.Fetch())
						tempfield = sql[2];
					if (IsNull(sql[2]))
					tempfield = 0;
					grid.Set(row, year + 2, 100 * tempfield / totalarea);
				}
				
			}
		}
		
		if (radio1.GetData() == 1)
		{
			if (radio2.GetData() == 0)
			{
				totalarea = 100;
			}
			if (radio2.GetData() == 1)
			{
				sql.Execute("SELECT Sum(Fields.Area) FROM Fields");
				if(sql.Fetch())
				totalarea = sql[0];
			}
			sql.Execute("SELECT Technologies.ID, Cultures.Name, Technologies.Urogai FROM Cultures INNER JOIN Technologies ON Cultures.ID = Technologies.Culture ORDER BY Cultures.Name");
			while(sql.Fetch())
			{
				grid.Add(sql[0]);
				text=sql[1]; 
				text<<", ";
				text<<sql[2];
				text<<" т/га";
				grid.Set(row, 1, text);
				row++;
			}
			for (int row = 0; row < grid.GetCount(); row++)
			{
				for (int year = 1; year <= Horizont; year++)
				{ 
					Year = AsString(year);
					sqlrequest = "SELECT Technologies.ID, Sum(Fields.Area) FROM Technologies LEFT JOIN Fields ON Technologies.ID = Fields.Year" + Year + " WHERE Technologies.ID = ";
					sqlrequest += AsString(grid.Get(row, 0));
					sqlrequest += " GROUP BY Technologies.ID";
					sql.Execute(sqlrequest);
					if(sql.Fetch())
						tempfield = sql[1];
					if (IsNull(sql[1]))
						tempfield = 0;
					tempfield = 100 * tempfield / totalarea;
					grid.Set(row, year + 2, tempfield);
				}
			}
		}

		if (radio1.GetData() == 2)
		{
			if (radio2.GetData() == 0)
			{
				totalarea = 100;
			}
			if (radio2.GetData() == 1)
			{
				sql.Execute("SELECT Sum(Fields.Area) FROM Fields");
				if(sql.Fetch())
				totalarea = sql[0];
			}
			sql.Execute("SELECT * FROM CultureType ORDER BY Name");
			while(sql.Fetch())
			{
				grid.Add(sql[0]);
				grid.Set(row, 1, sql[1]);
				row++;
			}
			for (int row = 0; row < grid.GetCount(); row++)
			{
				sqlrequest = "SELECT CultureType.ID, Sum(Fields.Area) FROM (CultureType LEFT JOIN Cultures ON CultureType.ID = Cultures.Type) LEFT JOIN Fields ON Cultures.ID = Fields.Culture WHERE CultureType.ID = ";
				sqlrequest += AsString(grid.Get(row, 0));
				sqlrequest += " GROUP BY CultureType.ID";
				sql.Execute(sqlrequest);
				if(sql.Fetch())
					tempfield = sql[1];
				if (IsNull(sql[1]))
					tempfield = 0;
				grid.Set(row, 2, 100 * tempfield / totalarea);
				for (int year = 1; year <= Horizont; year++)
				{ 
					Year = AsString(year);
					sqlrequest = "SELECT CultureType.ID, Sum(Fields.Area) FROM ((CultureType LEFT JOIN Cultures ON CultureType.ID = Cultures.Type) LEFT JOIN Technologies ON Cultures.ID = Technologies.Culture) LEFT JOIN Fields ON Technologies.ID = Fields.Year" + Year + " WHERE CultureType.ID = ";
					sqlrequest += AsString(grid.Get(row, 0));
					sqlrequest += " GROUP BY CultureType.ID";
					sql.Execute(sqlrequest);
					if(sql.Fetch())
						tempfield = sql[1];
					if (IsNull(sql[1]))
						tempfield = 0;
					grid.Set(row, year + 2, 100 * tempfield / totalarea);
				}
			}
		}
		
	}

    typedef PanPosiv CLASSNAME;
 	PanPosiv()
 	{
 		CtrlLayout(*this);
		CtrlLayout(sidectrl);
 		sidectrl.Add(radio1.LeftPosZ(16, 154).TopPosZ(16, 72));
 		radio1.SetLabel("Культури\nТехнології\nТипи культур");
 		radio1.SetData(0);
 		sidectrl.Add(radio2.LeftPosZ(16, 152).TopPosZ(104, 36));
 		radio2.SetLabel("абсолютні значення, га\nвідносні значення, %");	
 		radio2.SetData(0);
 		split.Horz(grid, sidectrl);
 		split.SetPos(7800);
 		Add(split.SizePos()); 		
		/*
		grid.AddIndex(ID);
 		grid.AddColumn("Показник", 150);
 		grid.AddColumn("Поточний рік", 100).AlignRight().SetConvert(Single<ConvDouble>()).DoSum();
		grid.AddColumn("Рік 1", 100).AlignRight().SetConvert(Single<ConvDouble>()).DoSum();
		grid.AddColumn("Рік 2", 100).AlignRight().SetConvert(Single<ConvDouble>()).DoSum();
		grid.AddColumn("Рік 3", 100).AlignRight().SetConvert(Single<ConvDouble>()).DoSum();
		grid.AddColumn("Рік 4", 100).AlignRight().SetConvert(Single<ConvDouble>()).DoSum();
		grid.AddColumn("Рік 5", 100).AlignRight().SetConvert(Single<ConvDouble>()).DoSum();
		grid.Proportional().Searching().SummaryRow(true).ColorRows(true);
		*/
		grid.Searching().Absolute().SummaryRow(true).ColorRows(true);
		radio1.WhenAction = THISBACK(Radio1);
		radio2.WhenAction = THISBACK(Radio2);
	};
};

#endif
