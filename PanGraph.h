#ifndef _AgroHold_PanGraph_h_
#define _AgroHold_PanGraph_h_

#include <ScatterCtrl/ScatterCtrl.h>

struct node : Moveable<node>
{
	int TechID;
	int Year;
};

class PanGraph : public WithPanelLayout<ParentCtrl> 
{   	
	public:
	int Horizont;
	int cyear;
	Button btnRozr, btnData;
	LineEdit lnedit;
	TreeCtrl tree;
	GridCtrl grid;
	Splitter split, split1;
	Sql sql;
	Id ID;
	WithPanelLayout<ParentCtrl> mainctrl;
	EditDate edDate;
	DropGrid edMTA;
	EditInt edDuration;
	Vector<node> treenodes;
		
	void BtnDataClick()
	{
		
		PromptOK("btnclicked");
	}
		
	// Кнопка расчета графика работ
	void BtnRozrClick()
	{
		sql.Execute("DELETE FROM Graphik");
		int newid = 0;
		String sqlrequest = "";
		for (int i = 1; i < treenodes.GetCount(); i++)
		{
			newid++;
			sql.Execute("INSERT INTO Graphik VALUES ('" + AsString(newid) + "', '" + AsString(treenodes[i].Year) + "', '" + AsString(treenodes[i].TechID) + "', '10.05.2014', '1', '2', '5')");
		}
		
		//sql.Execute("BEGIN TRANSACTION;");
		//sql.Execute(sqlrequest);
		//sql.Execute("COMMIT;");
		PromptOK("done");
	}
	
	void Update()
	{
		try
		{
			String sqlrequest;
			sqlrequest = "UPDATE Graphik SET Date = '" + AsString(grid(1)) +  "', MTA = '" + AsString(grid(4)) + "', Duration = '" + AsString(grid(6))+ "' WHERE ID = " + AsString(grid(0));
			sql.Execute(sqlrequest);
			sql.Execute("SELECT Norm_daily FROM MTA WHERE ID = '" + AsString(grid(4)) + "'");
			if(sql.Execute())
				grid(5) = (double)sql[0];
			grid(7) = (double)grid(3)/(double)grid(5)/(double)grid(6);
			
		}
		catch(SqlExc &e)
		{
			grid.CancelUpdate();
			Exclamation("[* " + DeQtfLf(e) + "]");
		}
	}
	
	void Populate()
	{
		// установим горизонт планирования
		sql.Execute("SELECT * FROM Project WHERE Param = 'Horizont'");
		if(sql.Fetch())
			Horizont = sql[1];
		// установим начальный год
		sql.Execute("SELECT * FROM Project WHERE Param = 'Year'");
		if(sql.Fetch())
			cyear = sql[1];
		// заполнение главного дерева
		tree.Clear();
		treenodes.Clear();
		grid.Clear();
		edMTA.Clear();
		tree.SetRoot(Images::Folder(), "Технології"); 			// Встановлюємо кореневу папку 0
		tree.Open(0);
		node& t = treenodes.Add();
		t.TechID = 0;
		t.Year = 0;
		
		int ind = 0; 
		int fold = 0;
		for (int i = 1; i <= Horizont; i++)
		{
			tree.Add(0, Images::Folder(), AsString(cyear + i));
			node& t = treenodes.Add();
			t.TechID = 0;
			t.Year = i;
			ind++;
			fold = ind;
			sql.Execute("SELECT Technologies.ID, Cultures.Name, Technologies.Urogai FROM Cultures, Technologies, Fields WHERE Technologies.ID = Fields.Year" + AsString(i) + " AND Cultures.ID = Technologies.Culture GROUP BY Technologies.ID ORDER BY Cultures.Name");
			while(sql.Fetch())	
			{
				tree.Add(fold, Images::File(), AsString(sql[1]) + ", "+ AsString(sql[2]) + " т/га");
				node& t = treenodes.Add();
				t.TechID = sql[0];
				t.Year = i;
				ind++;	
			}
			tree.Open(fold);
		}	
	}
	
	void TreeClick()
	{
		grid.Clear();
		double area;
		//Площа обраної технології у даний рік
		sql.Execute("SELECT Sum(Fields.Area) FROM Technologies LEFT JOIN Fields ON Technologies.ID = Fields.Year" + AsString(treenodes[tree.GetCursor()].Year) + " WHERE Technologies.ID = " + AsString(treenodes[tree.GetCursor()].TechID));
		if(sql.Fetch())
			area = (double)sql[0];
		// Заповнення таблиці календарного плану
		sql.Execute("SELECT Graphik.ID, Graphik.Date, Processes.ID_operation, Processes.Obsyag, Graphik.MTA, MTA.Norm_daily, Graphik.Duration FROM Graphik, Processes, MTA WHERE Graphik.Process = Processes.ID AND Graphik.MTA = MTA.ID AND Graphik.Year = " + AsString(treenodes[tree.GetCursor()].Year) + " AND Graphik.Technology = " + AsString(treenodes[tree.GetCursor()].TechID));
		int row = 0;
		Date date;
		double MTAcount = 0;
		while(sql.Fetch())
		{
			StrToDate(date, AsString(sql[1]));
			MTAcount = area * (double)sql[3]/(double)sql[5]/(double)sql[6] / 100;
			grid.Add(sql[0], sql[1], sql[2], area * (double)sql[3] / 100, sql[4], sql[5], sql[6], MTAcount);
		}
		// Заповнення випадаючого списку агрегатів
		edMTA.Clear();
		sql.Execute("SELECT MTA.ID, Energo.Name, Machinery.Name FROM MTA, Energo, Machinery WHERE MTA.ID_Tractor = Energo.ID AND MTA.ID_Machinery = Machinery.ID AND (MTA.Culture = '7' OR MTA.Culture = '')");
		while(sql.Fetch())
			edMTA.Add(sql[0],AsString(sql[1]) + " & " + AsString(sql[2]));
	}
			
    typedef PanGraph CLASSNAME;
 	PanGraph()
 	{		
 		CtrlLayout(*this);
 		CtrlLayout(mainctrl);
 		split1.Horz(tree,grid);
 		split1.SetPos(2200);
		mainctrl.Add(btnData.LeftPosZ(5, 90).TopPosZ(15, 28));
 		btnData.SetLabel("Аналіз");
 		mainctrl.Add(btnRozr.LeftPosZ(100, 90).TopPosZ(15, 28));
 		btnRozr.SetLabel("Розрахунок");
 		mainctrl.Add(lnedit.HSizePosZ(0, 5).VSizePosZ(55, 5));
 		split.Vert(split1, mainctrl);
		split.SetPos(7500);
 		Add(split.SizePos());
 		
		grid.AddIndex(ID);
		grid.AddColumn("Початок", 70).Edit(edDate);
		grid.AddColumn("Операція", 130).SetConvert(Single<OperationConvert>());
		grid.AddColumn("Обсяг", 80).AlignRight().SetConvert(Single<ConvDouble>());
		grid.AddColumn("Машино-тракторний агрегат", 160).Edit(edMTA).SetConvert(Single<MTAConvert>());
		grid.AddColumn("Норма виробітку", 95).AlignRight().SetConvert(Single<ConvDouble>());
		grid.AddColumn("Тривалість, дн.", 85).Edit(edDuration).AlignRight();
		grid.AddColumn("Кількість МТА", 80).AlignRight().SetConvert(Single<ConvDouble>());
		grid.Editing().Accepting().Canceling().Indicator().Searching().Absolute();
		grid.SetToolBar();
		
		// Таблиця випадаючого списку МТА					
		edMTA.AddColumn("ID").Width(0);
		edMTA.AddColumn("Агрегат", 110);
		edMTA.SetKeyColumn(0);
		edMTA.SetValueColumn(1);
		
		grid.WhenUpdateRow = THISBACK(Update);
		lnedit.SetFont(Monospace());
		
		btnRozr.WhenAction = THISBACK(BtnRozrClick);
		btnData.WhenAction = THISBACK(BtnDataClick);
		
		tree.WhenAction = THISBACK(TreeClick);
		
	};
};


#endif
