#ifndef _AgroHold_PanTech_h_
#define _AgroHold_PanTech_h_

class PanTech : public WithPanelLayout<ParentCtrl> {
	
	GridCtrl grid, grid2, grid3;
	Splitter 	split;				// Сплит на формі 
	Splitter 	split2;				// Сплит на формі 
	TabCtrl tab; 					// Таб процесів та матеріалів
	Sql sql;
	Id ID, ID2, ID3;
	String TableName, TableName2, TableName3;	// Назва таблиць
	
	DropGrid edCulture;				// Назва культури
	EditDouble edUrogai;			// Урожайність
	EditDouble edCina;				// Ціна
	EditDouble edCost;				// Витрати
	EditString edComment;			// Коментарій
	
	DropGrid edMonth;				// Місяць процесу
	DropGrid edOperation;			// Назва операції
	EditDouble edObsyag;			// Обсяг робіт на 100 га
	
	DropGrid edMonth2;				// Місяць процесу
	DropGrid edMater;				// Назва матеріалу
	EditDouble edMatNorm;			// Норма матеріалу на 1 га
	
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
			grid3.Enable(true);
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
				sql.Execute("DELETE FROM [" + TableName2 + "] WHERE ID_tech =" + AsString(grid(0)));
				sql.Execute("DELETE FROM [" + TableName3 + "] WHERE ID_tech =" + AsString(grid(0)));
				sql.Execute("DELETE FROM [" + TableName + "] WHERE ID=" + AsString(grid(0)));
				if (grid.GetRowCount() <= 1)
				{
					grid2.Enable(false);
					grid2.Clear();
					grid3.Enable(false);
					grid3.Clear();
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
	
	void BeforInsert()
	{
		grid2.Clear();
		grid3.Clear();
		grid2.Enable(false);
		grid3.Enable(false);
	}
	
	void TechSelect()
	{
		Sql sql;
		grid2.Clear();
		grid2.Enable(true);
		int row = 0;
		sql.Execute("SELECT * FROM Processes Where ID_tech = "  + AsString(grid(0)) + " ORDER BY ID");
		while(sql.Fetch())
		{
			grid2.Add(sql[0]);
			grid2.Set(row, 1, sql[2]);
			grid2.Set(row, 2, sql[3]);
			grid2.Set(row, 3, sql[4]);
			row++;
		};
		grid3.Clear();
		grid3.Enable(true);
		row = 0;
		sql.Execute("SELECT * FROM MaterForTech Where ID_tech = "  + AsString(grid(0)) + " ORDER BY ID");
		while(sql.Fetch())
		{
			grid3.Add(sql[0]);
			grid3.Set(row, 1, sql[2]);
			grid3.Set(row, 2, sql[3]);
			grid3.Set(row, 3, sql[4]);
			row++;
		};
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
			sqlrequest = "INSERT INTO [" + TableName2 + "] VALUES ('" + AsString(newid) + "', '" + AsString(grid(0)) + "', '" + AsString(grid2(1)) + "', '" + AsString(grid2(2)) + "', '" + AsString(grid2(3))+ "')";
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
			sql.Execute("UPDATE [" + TableName2 + "] SET Month = '" + AsString(grid2(1)) + "', ID_operation ='" + AsString(grid2(2)) + "', Obsyag ='" + AsString(grid2(3)) + " ' WHERE ID = " + AsString(grid2(0)));
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
	
	void Insert3()
    {
		try
		{	
			String sqlrequest;
			sql.Execute("SELECT MAX(ID) FROM " + TableName3);
			sql.Fetch();
			int x = sql[0];
			int newid;
			if (x > 0) 
				newid = x + 1;
			else
				newid = 1;
			sqlrequest = "INSERT INTO [" + TableName3 + "] VALUES ('" + AsString(newid) + "', '" + AsString(grid(0)) + "', '" + AsString(grid3(1)) + "', '" + AsString(grid3(2)) + "', '" + AsString(grid3(3)) + "')";
			sql.Execute(sqlrequest);
			grid3(0) = newid;
		}
		catch(SqlExc &e)
		{
			grid3.CancelInsert();
			Exclamation("[* " + DeQtfLf(e) + "]");
		}
	}
	
	void Update3()
	{
		try
		{
			String sqlrequest;
			sql.Execute("UPDATE [" + TableName3 + "] SET Month = '" + AsString(grid3(1)) + "', ID_material ='" + AsString(grid3(2)) + "', Norm ='" + AsString(grid3(3)) +  " ' WHERE ID = " + AsString(grid3(0)));
		}
		catch(SqlExc &e)
		{
			grid3.CancelUpdate();
			Exclamation("[* " + DeQtfLf(e) + "]");
		}
	}
	
	void Remove3()
	{
		try
		{
			if (Prompt("Агроплан", CtrlImg::question(), "Видалити запис?", "Так", "Ні"))
			{
				sql.Execute("DELETE FROM [" + TableName3 + "] WHERE ID = " + AsString(grid3(0)));
			}
			else
			{
				grid3.CancelRemove();
			}
		}
		catch(SqlExc &e)
		{
			grid3.CancelRemove();
			Exclamation("[* " + DeQtfLf(e) + "]");
		}
	}
	
public:        	
	
	void Populate()
	{
		// заполнение выпадающего списка операцій
		edOperation.Clear();
		sql.Execute("SELECT * FROM Operations ORDER BY Name");
		while(sql.Fetch())
			edOperation.Add(sql[0],sql[1],sql[2]);
		// заполнение выпадающего списка ТМЦ
		edMater.Clear();
		sql.Execute("SELECT * FROM Materials ORDER BY Name");
		while(sql.Fetch())
			edMater.Add(sql[0],sql[1],sql[3]);
		// заполнение выпадающего списка  культуры
		edCulture.Clear();
		sql.Execute("SELECT * FROM Cultures ORDER BY Name");
		while(sql.Fetch())
			edCulture.Add(sql[0],sql[1]);
		// заполнение таблицы
		grid.Clear();
		sql.Execute("SELECT * FROM " + TableName + " ORDER BY Culture");
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
		grid3.Clear();
		grid2.Enable(false);
		grid3.Enable(false);
	}

    typedef PanTech CLASSNAME;
 	PanTech()
 	{
 		CtrlLayout(*this);	
 		split.Vert(grid, tab);
 		tab.Add(grid2.SizePos(), "Процеси");
 		tab.Add(grid3.SizePos(), "ТМЦ");
 		split.SetPos(5000);
 		Add(split.SizePos());
 		
 		TableName = "Technologies";
		TableName2 = "Processes";
		TableName3 = "MaterForTech";
		
		grid.AddIndex(ID);
		grid.AddColumn("Культура", 150).Edit(edCulture).SetConvert(Single<CultConvert>());
		grid.AddColumn("Урожайність, т/га", 140).Edit(edUrogai).AlignRight().SetConvert(Single<ConvDouble>());
		grid.AddColumn("Ціна, грн./т", 140).Edit(edCina).AlignRight().SetConvert(Single<ConvDouble>());
		grid.AddColumn("Витрати, грн./га", 140).Edit(edCost).AlignRight().SetConvert(Single<ConvDouble>());
		grid.AddColumn("Примітка", 200).Edit(edComment);
		grid.WhenInsertRow = THISBACK(Insert);
		grid.WhenUpdateRow = THISBACK(Update);
		grid.WhenRemoveRow = THISBACK(Remove);
		grid.WhenChangeRow = THISBACK(TechSelect);
		grid.WhenBeforeChangeRow = THISBACK(BeforInsert);
		grid.Appending().Removing().Editing().Accepting().Canceling().Indicator().Searching().Absolute().SetToolBar();

		grid2.AddIndex(ID2);
		grid2.AddColumn("Місяць", 140).Edit(edMonth).SetConvert(Single<MonthConvert>());
		grid2.AddColumn("Назва операції", 140).Edit(edOperation).SetConvert(Single<OperationConvert>());
		grid2.AddColumn("Обсяг робіт на 100 га", 140).Edit(edObsyag).AlignRight().SetConvert(Single<ConvDouble>());
		grid2.WhenInsertRow = THISBACK(Insert2);
		grid2.WhenUpdateRow = THISBACK(Update2);
		grid2.WhenRemoveRow = THISBACK(Remove2);
		grid2.Appending().Removing().Editing().Accepting().Canceling().Indicator().Searching().Absolute().SetToolBar();		
		grid2.Enable(false);
		
		grid3.AddIndex(ID3);
		grid3.AddColumn("Місяць", 140).Edit(edMonth2).SetConvert(Single<MonthConvert>());
		grid3.AddColumn("Назва ТМЦ", 140).Edit(edMater).SetConvert(Single<MaterConvert>());
		grid3.AddColumn("Норма матеріалу на 1 га", 140).Edit(edMatNorm).AlignRight().SetConvert(Single<ConvDouble>());
		grid3.WhenInsertRow = THISBACK(Insert3);
		grid3.WhenUpdateRow = THISBACK(Update3);
		grid3.WhenRemoveRow = THISBACK(Remove3);
		grid3.Appending().Removing().Editing().Accepting().Canceling().Indicator().Searching().Absolute().SetToolBar();
		grid3.Enable(false);
		
		// Таблиця випадаючого списку культури					
		edCulture.AddColumn("ID").Width(0);
		edCulture.AddColumn("Культура", 110);
		edCulture.SetKeyColumn(0);
		edCulture.SetValueColumn(1);
		
		// Таблиця випадаючого списку операцій					
		edOperation.AddColumn("ID").Width(0);
		edOperation.AddColumn("Операція", 100);
		edOperation.AddColumn("Од.вим.", 50);
		edOperation.SetKeyColumn(0);
		edOperation.SetValueColumn(1);
		
		// Таблиця випадаючого списку матеріалів					
		edMater.AddColumn("ID").Width(0);
		edMater.AddColumn("Назва ТМЦ", 0);
		edMater.AddColumn("Од.вим.", 50);
		edMater.SetKeyColumn(0);
		edMater.SetValueColumn(1);
				
		// Таблиця випадаючого списку місяцю 1
		edMonth.AddColumn("ID").Width(0);
		edMonth.AddColumn("Місяць", 110);
		edMonth.SetKeyColumn(0);
		edMonth.SetValueColumn(1);
		
		// Таблиця випадаючого списку місяцю 2
		edMonth2.AddColumn("ID").Width(0);
		edMonth2.AddColumn("Місяць", 80);
		edMonth2.SetKeyColumn(0);
		edMonth2.SetValueColumn(1);
		
		// Заповнення місцяів		
		edMonth.Add(1,"підготовка: липень");
		edMonth.Add(2,"підготовка: серпень");
		edMonth.Add(3,"підготовка: вересень");
		edMonth.Add(4,"підготовка: жовтень");
		edMonth.Add(5,"січень");
		edMonth.Add(6,"лютий");
		edMonth.Add(7,"березень");
		edMonth.Add(8,"квітень");
		edMonth.Add(9,"травень");
		edMonth.Add(10,"червень");
		edMonth.Add(11,"липень");
		edMonth.Add(12,"серпень");
		edMonth.Add(13,"вересень");
		edMonth.Add(14,"жовтень");
		edMonth.Add(15,"листопад");
		edMonth.Add(16,"грудень");
		
		edMonth2.Add(1,"підготовка: липень");
		edMonth2.Add(2,"підготовка: серпень");
		edMonth2.Add(3,"підготовка: вересень");
		edMonth2.Add(4,"підготовка: жовтень");
		edMonth2.Add(5,"січень");
		edMonth2.Add(6,"лютий");
		edMonth2.Add(7,"березень");
		edMonth2.Add(8,"квітень");
		edMonth2.Add(9,"травень");
		edMonth2.Add(10,"червень");
		edMonth2.Add(11,"липень");
		edMonth2.Add(12,"серпень");
		edMonth2.Add(13,"вересень");
		edMonth2.Add(14,"жовтень");
		edMonth2.Add(15,"листопад");
		edMonth2.Add(16,"грудень");
	};
};

#endif
