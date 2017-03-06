#ifndef _AgroHold_PanMaterials_h_
#define _AgroHold_PanMaterials_h_

class PanMaterials : public WithPanelLayout<ParentCtrl> {
	
	GridCtrl grid;
	Sql sql;
	Id ID;
	String TableName;
	EditString edName;
	DropGrid edMatType;
	EditString edOdvim;
	EditDouble edPrice;
	
		
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
		edMatType.Clear();
		Sql sql;
		sql.Execute("SELECT * FROM MatType ORDER BY Name");
		while(sql.Fetch())
			edMatType.Add(sql[0],sql[1]);
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

    typedef PanMaterials CLASSNAME;
 	PanMaterials()
 	{
 		CtrlLayout(*this);	
 		Add(grid.SizePos());
 		TableName = "Materials";
		grid.AddIndex(ID);
		grid.AddColumn("Назва матеріалу", 200).Edit(edName);
		grid.AddColumn("Тип ТМЦ", 160).Edit(edMatType).SetConvert(Single<MatTypeConvert>());
		grid.AddColumn("Одиниця виміру", 110).Edit(edOdvim);
		grid.AddColumn("Ціна за одиницю, грн.", 110).AlignRight().Edit(edPrice).SetConvert(Single<ConvDouble>());
		// Таблиця випадаючого списку господарств					
		edMatType.AddColumn("ID").Width(0);
		edMatType.AddColumn("Тип ТМЦ", 110);
		edMatType.SetKeyColumn(0);
		edMatType.SetValueColumn(1);
		grid.WhenInsertRow = THISBACK(Insert);
		grid.WhenUpdateRow = THISBACK(Update);
		grid.WhenRemoveRow = THISBACK(Remove);
		grid.Appending().Removing().Editing().Accepting().Canceling().Indicator().Searching().Absolute().SetToolBar();
	};
};

#endif
