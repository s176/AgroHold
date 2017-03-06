#ifndef _AgroHold_PanPop_h_
#define _AgroHold_PanPop_h_

class PanPop : public WithPanelLayout<ParentCtrl> {
	
	GridCtrl grid;
	Sql sql;
	Id ID;
	String TableName;
	DropGrid edPopName;
	DropGrid edNastName;
	EditDouble edVpliv; 
		
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
				sql.Execute("DELETE FROM [" + TableName + "] WHERE ID=" + AsString(grid(0)));
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
	
public:        	
	
	void Populate()
	{
		// заполнение выпадающего списка
		edPopName.Clear();
		edNastName.Clear();
		Sql sql;
		sql.Execute("SELECT * FROM Cultures ORDER BY Name");
		while(sql.Fetch())
		{
			edPopName.Add(sql[0],sql[1]);
			edNastName.Add(sql[0],sql[1]);
		}
		// заполнение таблицы
		grid.Clear();
		sql.Execute("SELECT * FROM " + TableName);
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
	}

    typedef PanPop CLASSNAME;
 	PanPop()
 	{
 		CtrlLayout(*this);	
 		Add(grid.SizePos());
 		TableName = "Poperedniki";
		grid.AddIndex(ID);
		grid.AddColumn("Культура-попередник", 150).Edit(edPopName).SetConvert(Single<CultConvert>());
		grid.AddColumn("Культура-наступник", 150).Edit(edNastName).SetConvert(Single<CultConvert>());
		grid.AddColumn("Коефіцієнт вливу", 110).AlignRight().Edit(edVpliv).SetConvert(Single<ConvDouble>());
		// Таблиця випадаючого списку					
		edPopName.AddColumn("ID").Width(0);
		edPopName.AddColumn("Попередник", 110);
		edPopName.SetKeyColumn(0);
		edPopName.SetValueColumn(1);
		// Таблиця випадаючого списку					
		edNastName.AddColumn("ID").Width(0);
		edNastName.AddColumn("Культура", 110);
		edNastName.SetKeyColumn(0);
		edNastName.SetValueColumn(1);
		grid.WhenInsertRow = THISBACK(Insert);
		grid.WhenUpdateRow = THISBACK(Update);
		grid.WhenRemoveRow = THISBACK(Remove);
		grid.Appending().Removing().Editing().Accepting().Canceling().Indicator().Searching().Absolute().SetToolBar();
	};
};


#endif
