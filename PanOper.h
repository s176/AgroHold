#ifndef _AgroHold_PanOper_h_
#define _AgroHold_PanOper_h_

class PanOper : public WithPanelLayout<ParentCtrl> {
	
	GridCtrl grid, grid2;
	Splitter 	split;				// Сплит на формі 
	Sql sql;
	Id ID, ID2;
	String TableName, TableName2;	// Назва таблиць
	EditString edName;				// Назва операції
	EditString edOdvim;				// Одиниця виміру
	EditInt edDuration;				// Тривалість операції 
	
	DropGrid edEnergoName;			// Назва трактора
	DropGrid edMashName;			// Назва машини
	EditDouble edNormDaily;			// Змінна (денна) норма виробітку
	EditDouble edOP;				// Оплата праці, грн. на 1 га
	EditDouble edPalne;				// Витрати на пальне, грн. на 1 га
	DropGrid edForCulture;			// Назва культури, для якої ця операція, якщо пусто - для будь-якої
			
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
				sql.Execute("DELETE FROM [" + TableName2 + "] WHERE ID_operation =" + AsString(grid(0)));
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
	
	void OperationSelect()
	{
		grid2.Clear();
		grid2.Enable(true);
		Sql sql;
		int row = 0;
		sql.Execute("SELECT * FROM MTA Where ID_operation = "  + AsString(grid(0)));
		while(sql.Fetch())
		{
			grid2.Add(sql[0]);
			grid2.Set(row, 1, sql[2]);
			grid2.Set(row, 2, sql[3]);
			grid2.Set(row, 3, sql[4]);
			grid2.Set(row, 4, sql[5]);
			grid2.Set(row, 5, sql[6]);
			grid2.Set(row, 6, sql[7]);
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
			sqlrequest = "INSERT INTO [" + TableName2 + "] VALUES ('" + AsString(newid) + "', '" + AsString(grid(0)) + "', '" + AsString(grid2(1)) + "', '" + AsString(grid2(2)) + "', '" + AsString(grid2(3))+ "', '" + AsString(grid2(4))+ "', '" + AsString(grid2(5))+ "', '" + AsString(grid2(6)) + "')";
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
			sql.Execute("UPDATE [" + TableName2 + "] SET ID_Tractor = '" + AsString(grid2(1)) + "', ID_Machinery ='" + AsString(grid2(2)) + "', Norm_daily ='" + AsString(grid2(3)) + "', OP ='" + AsString(grid2(4)) + "', Palne ='" + AsString(grid2(5)) + "', Culture ='" + AsString(grid2(6)) +  " ' WHERE ID = " + AsString(grid2(0)));
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
		// заполнение выпадающего списка енергомашин
		edEnergoName.Clear();
		sql.Execute("SELECT * FROM Energo ORDER BY Name");
		while(sql.Fetch())
			edEnergoName.Add(sql[0],sql[1]);
		// заполнение выпадающего списка с.г. машин
		edMashName.Clear();
		sql.Execute("SELECT * FROM Machinery ORDER BY Name");
		while(sql.Fetch())
			edMashName.Add(sql[0],sql[1]);
		// заполнение выпадающего списка  культуры
		edForCulture.Clear();
		sql.Execute("SELECT * FROM Cultures ORDER BY Name");
		while(sql.Fetch())
			edForCulture.Add(sql[0],sql[1]);
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
	}

    typedef PanOper CLASSNAME;
 	PanOper()
 	{
 		CtrlLayout(*this);	
 		split.Vert(grid, grid2);
 		split.SetPos(5000);
 		Add(split.SizePos());
 		TableName = "Operations";
		TableName2 = "MTA";
		grid.AddIndex(ID);
		grid.AddColumn("Назва операції", 150).Edit(edName);
		grid.AddColumn("Одиниця виміру", 150).Edit(edOdvim);
		grid.AddColumn("Тривалість, днів", 120).Edit(edDuration).AlignRight();
		grid.WhenInsertRow = THISBACK(Insert);
		grid.WhenUpdateRow = THISBACK(Update);
		grid.WhenRemoveRow = THISBACK(Remove);
		grid.WhenChangeRow = THISBACK(OperationSelect);
		grid.WhenBeforeChangeRow = THISBACK(BeforInsert);
		grid.Appending().Removing().Editing().Accepting().Canceling().Indicator().Searching().Absolute().SetToolBar();

		grid2.AddIndex(ID2);
		grid2.AddColumn("Енергомашина", 150).Edit(edEnergoName).SetConvert(Single<EnergoConvert>());
		grid2.AddColumn("С.-г. машина", 150).Edit(edMashName).SetConvert(Single<MashConvert>());
		grid2.AddColumn("Денна норма виробітку", 140).Edit(edNormDaily).AlignRight().SetConvert(Single<ConvDouble>());
		grid2.AddColumn("Оплата праці, грн./га", 140).Edit(edOP).AlignRight().SetConvert(Single<ConvDouble>());
		grid2.AddColumn("Витрати на пальне, грн./га", 140).Edit(edPalne).AlignRight().SetConvert(Single<ConvDouble>());
		grid2.AddColumn("Культура (пуста - операція для будь-якої)", 230).Edit(edForCulture).SetConvert(Single<CultConvert>());
		grid2.WhenInsertRow = THISBACK(Insert2);
		grid2.WhenUpdateRow = THISBACK(Update2);
		grid2.WhenRemoveRow = THISBACK(Remove2);
		grid2.Appending().Removing().Editing().Accepting().Canceling().Indicator().Searching().Absolute().SetToolBar();	
		grid2.Enable(false);	
		
		// Таблиця випадаючого списку енергомашини					
		edEnergoName.AddColumn("ID").Width(0);
		edEnergoName.AddColumn("Енергомашина", 110);
		edEnergoName.SetKeyColumn(0);
		edEnergoName.SetValueColumn(1);
		
		// Таблиця випадаючого списку с.г. машини					
		edMashName.AddColumn("ID").Width(0);
		edMashName.AddColumn("С.-г. машина", 110);
		edMashName.SetKeyColumn(0);
		edMashName.SetValueColumn(1);
				
		// Таблиця випадаючого списку культури					
		edForCulture.AddColumn("ID").Width(0);
		edForCulture.AddColumn("Культура", 110);
		edForCulture.SetKeyColumn(0);
		edForCulture.SetValueColumn(1);
	};
};

#endif
