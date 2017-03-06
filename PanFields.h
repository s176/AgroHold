#ifndef _AgroHold_PanFields_h_
#define _AgroHold_PanFields_h_

class PanFlds : public WithPanelLayout<ParentCtrl> {
	
	GridCtrl grid, grid2;
	Splitter 	split;				// Сплит на формі 
	Sql sql;
	Id ID, ID2;
	String TableName, TableName2;	// Назва таблиць
	EditString edName;				// Назва поля
	EditDouble 	edArea;				// Площа поля
	DropGrid 	edCurCulture;		// Назва поточної на полі культури
	DropGrid edType;				// Тип культури
	EditInt edBal;					// Бал бонітету 
		
    void Insert()
    {
		try
		{	
			String sqlrequest;
			sql.Execute("SELECT MAX(ID) FROM " + TableName);
			sql.Fetch();
			int x = sql[0];
			int newid;
			if (x > 0) 
				newid = x + 1;
			else
				newid = 1;
			sql.Execute("SELECT * FROM [" + TableName + "] LIMIT 1");
			for(int i = 1; i < sql.GetColumns(); i++)
			{	
				sqlrequest += AsString(grid(i)) + "'";
				if(i != sql.GetColumns() - 1)
				 	sqlrequest +=", '";
			}		
			sqlrequest = "INSERT INTO [" + TableName + "] VALUES ('" + AsString(newid) + "', '" + sqlrequest + ")";
			sql.Execute(sqlrequest);
			grid(0) = newid;
			grid2.Enable(true);
		}
		catch(SqlExc &e)
		{
			grid.CancelInsert();
			Exclamation("[* " + DeQtfLf(e) + "]");
		}
	}
	
	void Update()
	{
		try
		{
			String sqlrequest;
			sql.Execute("SELECT * FROM [" + TableName + "] LIMIT 1");
			for(int i = 1; i < sql.GetColumns(); i++)
			{	
				sqlrequest += sql.GetColumnInfo(i).name + "='" + AsString(grid(i)) + "' ";
				if(i != sql.GetColumns() - 1)
				 	sqlrequest +=",";
				PromptOK(sqlrequest);
			}
			sql.Execute("UPDATE [" + TableName + "] SET " + sqlrequest + "WHERE ID=" + AsString(grid(0)));
		}
		catch(SqlExc &e)
		{
			grid.CancelUpdate();
			Exclamation("[* " + DeQtfLf(e) + "]");
		}
	}
	void Remove()
	{
		try
		{
			if (Prompt("Агроплан", CtrlImg::question(), "Видалити запис?", "Так", "Ні"))
			{
				sql.Execute("DELETE FROM [" + TableName2 + "] WHERE Field = " + AsString(grid(0)));
				sql.Execute("DELETE FROM [" + TableName + "] WHERE ID=" + AsString(grid(0)));
				if (grid.GetRowCount() <= 1)
				{
					grid2.Enable(false);
					grid2.Clear();
				}
				
			}
			else
			{
				grid.CancelRemove();
			}
		}
		catch(SqlExc &e)
		{
			grid.CancelRemove();
			Exclamation("[* " + DeQtfLf(e) + "]");
		}
	}
	
	void FieldsSelect()
	{
		grid2.Clear();
		grid2.Enable(true);
		Sql sql;
		int row = 0;
		sql.Execute("SELECT * FROM FieldsBonitet Where Field = "  + AsString(grid(0)));
		while(sql.Fetch())
		{
			grid2.Add(sql[0]);
			grid2.Set(row, 1, sql[2]);
			grid2.Set(row, 2, sql[3]);
			row++;
		};
	}
	
	void BeforInsert()
	{
		grid2.Clear();
		grid2.Enable(false);
	}
	
	void Insert2()
    {
		try
		{	
			String sqlrequest;
			sql.Execute("SELECT MAX(ID) FROM " + TableName2);
			sql.Fetch();
			int x = sql[0];
			int newid;
			if (x > 0) 
				newid = x + 1;
			else
				newid = 1;
			sqlrequest = "INSERT INTO [" + TableName2 + "] VALUES ('" + AsString(newid) + "', '" + AsString(grid(0)) + "', '" + AsString(grid2(1)) + "', '" + AsString(grid2(2)) + "')";
			sql.Execute(sqlrequest);
			grid2(0) = newid;
		}
		catch(SqlExc &e)
		{
			grid2.CancelInsert();
			Exclamation("[* " + DeQtfLf(e) + "]");
		}
	}
	
	void Update2()
	{
		try
		{
			String sqlrequest;
			sql.Execute("UPDATE [" + TableName2 + "] SET CultureType = '" + AsString(grid2(1)) + "', Bal ='" + AsString(grid2(2)) +  " ' WHERE ID = " + AsString(grid2(0)));
		}
		catch(SqlExc &e)
		{
			grid2.CancelUpdate();
			Exclamation("[* " + DeQtfLf(e) + "]");
		}
	}
	void Remove2()
	{
		try
		{
			if (Prompt("Агроплан", CtrlImg::question(), "Видалити запис?", "Так", "Ні"))
			{
				sql.Execute("DELETE FROM [" + TableName2 + "] WHERE ID = " + AsString(grid2(0)));
			}
			else
			{
				grid2.CancelRemove();
			}
		}
		catch(SqlExc &e)
		{
			grid2.CancelRemove();
			Exclamation("[* " + DeQtfLf(e) + "]");
		}
	}
	
public:        	
	
	void Populate()
	{
		// заполнение выпадающего списка текущей культуры
		edCurCulture.Clear();
		sql.Execute("SELECT * FROM Cultures ORDER BY Name");
		while(sql.Fetch())
			edCurCulture.Add(sql[0],sql[1]);
		// заполнение выпадающего списка типа культур
		edType.Clear();
		sql.Execute("SELECT * FROM CultureType ORDER BY Name");
		while(sql.Fetch())
			edType.Add(sql[0],sql[1]);
		// заполнение таблицы
		grid.Clear();
		sql.Execute("SELECT * FROM " + TableName + " ORDER BY Name");
		int row = 0;
		while(sql.Fetch())
		{
			grid.Add(sql[0]);
			for (int i = 0; i < sql.GetColumns(); i++)		
			{
				grid.Set(row, i, sql[i]);
			}
		row++;
		}	
		grid2.Clear();
		grid2.Enable(false);		
	}

    typedef PanFlds CLASSNAME;
 	PanFlds()
 	{
 		CtrlLayout(*this);	
 		split.Vert(grid, grid2);
 		split.SetPos(5000);
 		Add(split.SizePos());
 		TableName = "Fields";
		TableName2 = "FieldsBonitet";
		grid.AddIndex(ID);
		grid.AddColumn("Назва (номер) поля", 150).Edit(edName);
		grid.AddColumn("Площа поля, га", 120).Edit(edArea).AlignRight().SetConvert(Single<ConvDouble>());
		grid.AddColumn("Поточна культура", 0).Edit(edCurCulture).SetConvert(Single<CultConvert>());
		grid.AddColumn("Рік 1", 0);
		grid.AddColumn("Рік 2", 0);
		grid.AddColumn("Рік 3", 0);
		grid.AddColumn("Рік 4", 0);
		grid.AddColumn("Рік 5", 0);
		grid.AddColumn("Рік 6", 0);
		grid.AddColumn("Рік 7", 0);
		grid.AddColumn("Рік 8", 0);
		grid.AddColumn("Рік 9", 0);
		grid.AddColumn("Рік 10", 0);
		grid.WhenInsertRow = THISBACK(Insert);
		grid.WhenUpdateRow = THISBACK(Update);
		grid.WhenRemoveRow = THISBACK(Remove);
		grid.WhenChangeRow = THISBACK(FieldsSelect);
		grid.WhenBeforeChangeRow = THISBACK(BeforInsert);
		grid.Appending().Removing().Editing().Accepting().Canceling().Indicator().Searching().Absolute().SetToolBar();

		grid2.AddIndex(ID2);
		grid2.AddColumn("Тип культури", 200).Edit(edType).SetConvert(Single<TypeConvert>());
		grid2.AddColumn("Бал бонітету", 120).Edit(edBal).AlignRight();
		grid2.WhenInsertRow = THISBACK(Insert2);
		grid2.WhenUpdateRow = THISBACK(Update2);
		grid2.WhenRemoveRow = THISBACK(Remove2);
		grid2.Appending().Removing().Editing().Accepting().Canceling().Indicator().Searching().Absolute().SetToolBar();
		grid2.Enable(false);		
		
		// Таблиця випадаючого списку поточної культури					
		edCurCulture.AddColumn("ID").Width(0);
		edCurCulture.AddColumn("Поточна культура", 110);
		edCurCulture.SetKeyColumn(0);
		edCurCulture.SetValueColumn(1);
		
		// Таблиця випадаючого списку типу культур					
		edType.AddColumn("ID").Width(0);
		edType.AddColumn("Тип культури", 110);
		edType.SetKeyColumn(0);
		edType.SetValueColumn(1);

	};
};

#endif
