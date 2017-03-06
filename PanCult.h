#ifndef _AgroHold_PanCult_h_
#define _AgroHold_PanCult_h_

class PanCult : public WithPanelLayout<ParentCtrl> {
	
	GridCtrl grid;
	Sql sql;
	Id ID;
	String TableName;
	EditString edName;
	DropGrid edType;
	EditInt edRotation; 
		
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
		edType.Clear();
		Sql sql;
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
	}

    typedef PanCult CLASSNAME;
 	PanCult()
 	{
 		CtrlLayout(*this);	
 		Add(grid.SizePos());
 		TableName = "Cultures";
		grid.AddIndex(ID);
		grid.AddColumn("Назва культури", 150).Edit(edName);
		grid.AddColumn("Тип культури", 150).Edit(edType).SetConvert(Single<TypeConvert>());		
		grid.AddColumn("Ротація, років", 80).AlignRight().Edit(edRotation);
		// Таблиця випадаючого списку господарств					
		edType.AddColumn("ID").Width(0);
		edType.AddColumn("Тип культури", 110);
		edType.SetKeyColumn(0);
		edType.SetValueColumn(1);
		grid.WhenInsertRow = THISBACK(Insert);
		grid.WhenUpdateRow = THISBACK(Update);
		grid.WhenRemoveRow = THISBACK(Remove);
		grid.Absolute();
		grid.Appending().Removing().Editing().Accepting().Canceling().Indicator().Searching();
		grid.SetToolBar();
	};
};

#endif
