#ifndef _AgroHold_PanSiv_h_
#define _AgroHold_PanSiv_h_

class PanSiv : public WithPanelLayout<ParentCtrl> 
{    	
	// класс ребер
    struct Myedge : Moveable<Myedge>
    {
        int PopVertexID;           // ссылка на код предшествующей вершины в графе
        int NastVertexID;          // ссылка на код последующей вершины в графе
        double value;              // величина чистого дохода по этому ребру
        double kumulvalue;         // кумулятивна величина чистого доходу
    };

    // класс вершин
    struct Myvertex : Moveable<Myvertex>
    {
        int ID;              // код данной вершины в графе
        int fieldID;         // код поля
        double area;		 // площа поля, га
        int cultureID;       // код культуры
        int technologyID;    // код технологии
        int typecultureID;   // код типа культуры
        int year;            // год
        int variable;		// змінна так 1, ні 0
		Vector<int> cultinpath; // список ID культур в пути до этой вершины
    };
    
    // класс типа культур
    struct Myculttypetable : Moveable<Myculttypetable>
    {
        int ID;
        int min;
        int max;
    };
    
    // класс культур
    struct Myculttable : Moveable<Myculttable>
    {
        int ID;
        int type;
        int rotation;
    };
	
	// класс поля
    struct Myfieldtable : Moveable<Myfieldtable>
    {
        int ID;
        double area;
        int culture;
    };
	
	// класс предшественники
    struct Mypopertable : Moveable<Mypopertable>
    {
        int ID;
        int IDpop;
        int IDnast;
        double vpliv;
	};
	
	// класс бонитета
    struct Myfieldbontable : Moveable<Myfieldbontable>
    {
        int ID;
        int field;
        int culturetype;
        int bal;
	};
		
	// класс технологий
    struct Mytechtable : Moveable<Mytechtable>
    {
        int ID;
        int CultureID;
        double Urogai;
		double Cina;
		double Cost;
    };
	
	// класс змінних
    struct Myvar : Moveable<Myvar>
    {
        int column;
		double area;
    };
	int Horizont; // горизонт планування - скільки років плануємо
	Button btnRozr, btnData;
	Option optSaveGrapf;
	LineEdit lnedit;
	GridCtrl grid;
	Splitter 	split;
	Sql sql;
	Id ID;
	String TableName;	// Назва таблиць
	EditString edName;				// Назва поля
	EditDouble 	edArea;				// Площа поля
	DropGrid 	edCurCulture;		// Назва поточної на полі культури
	DropGrid 	edTech1;			// ID технології на полі у рік 1
	DropGrid 	edTech2;			// ID технології на полі у рік 2
	DropGrid 	edTech3;			// ID технології на полі у рік 3
	DropGrid 	edTech4;			// ID технології на полі у рік 4
	DropGrid 	edTech5;			// ID технології на полі у рік 5
	DropGrid 	edTech6;			// ID технології на полі у рік 6
	DropGrid 	edTech7;			// ID технології на полі у рік 7
	DropGrid 	edTech8;			// ID технології на полі у рік 8
	DropGrid 	edTech9;			// ID технології на полі у рік 9
	DropGrid 	edTech10;			// ID технології на полі у рік 10
	WithPanelLayout<ParentCtrl> mainctrl;
	
	int n;			        // индекс вершины
	Vector<Myvertex> GraphVertex;
    Vector<Myedge> GraphEdge;
    Vector<Mytechtable> techtable;
	Vector<Myfieldbontable> fieldbontable;
	Vector<Mypopertable> popertable;
	Vector<Myfieldtable> fieldtable;
	Vector<Myculttable> culttable;
	Vector<Myculttypetable> culttypetable;
	
public:        	
	
	// тип культуры по коду культуры
	int culturetype(int ID_cult)
	{
		//Sql sql;
		//sql.Execute("SELECT * FROM Cultures Where ID = " + AsString(ID_cult));
		for (int i = 0; i < culttable.GetCount(); i++)
		{
			if (culttable[i].ID == ID_cult)
				return culttable[i].type;	
		}
		return 0;
	}
	
	// Кнопка для предварительно анализа данных
	void BtnDataClick()
	{
		TimeStop tm; // лічильник часу використаного на даний аналіз
		String report;// = "Звіт про попередній аналіз даних щодо можливості розрахунку сівозміни.\n";
		int i; // временный счетчик количества полей в данных
		int err; // временный счетчик количества ошибок в таблицах
		sql.Execute("SELECT * FROM CultureType");
		report += "Довідник 'Типи культур' ... ";
		err = 0; i = 0;
		double min, max;
		while(sql.Fetch())
		{	
			min = sql[2];
			max = sql[3];
			if (IsNull(sql[2]))
				{report += "\n--- для типу культур '" + AsString(sql[1]) + "' не визначено мінімальне значення."; err++;}
			if (IsNull(sql[3]))
				{report += "\n--- для типу культур '" + AsString(sql[1]) + "' не визначено максимальне значення."; err++;}
			if (min > max || min < 0 || min > 100 || max < 0 || max > 100)
				{report += "\n--- для типу культур '" + AsString(sql[1]) + "' недопустимий діапазон значень [0..100]."; err++;}
			i++;
		}
		// Список не должен быть пустым
		if (i == 0)
			{report += "\n--- перелік типів культур не містить жодного запису. "; err++;}
		if (err == 0) 
			report += "Ok.";
		
		sql.Execute("SELECT * FROM Cultures");
		report += "\nДовідник 'Культури' ... ";
		err = 0; i = 0;
		while(sql.Fetch())
		{	
			if (IsNull(sql[2]))
				{report += "\n--- для культури '" + AsString(sql[1]) + "' не визначено тип."; err++;}
			if (IsNull(sql[3]))
				{report += "\n--- для культури '" + AsString(sql[1]) + "' не визначено ротацію."; err++;}
			i++;
		}
		// Список не должен быть пустым
		if (i == 0)
			{report += "\n--- перелік культур не містить жодного запису. \n"; err++;}
		if (err == 0) 
			report += "Ok.";
		
		sql.Execute("SELECT * FROM Fields");
		report += "\nДовідник 'Поля' ... ";
		err = 0; i = 0;
		while(sql.Fetch())
		{	
			if (IsNull(sql[2]))
				{report += "\n--- для поля '" + AsString(sql[1]) + "' не визначено площу."; err++;}
			if (IsNull(sql[3]))
				{report += "\n--- для поля '" + AsString(sql[1]) + "' не визначено поточну культуру."; err++;}
			i++;
		}
		sql.Execute("SELECT Fields.ID, Fields.Name, FieldsBonitet.Bal FROM Fields Left JOIN FieldsBonitet ON Fields.ID = FieldsBonitet.Field");
		int balbon;
		while(sql.Fetch())
		{
			balbon = sql[2];
			if (IsNull(sql[2]) || balbon < 0 || balbon > 100)
				{report += "\n--- для поля '" + AsString(sql[1]) + "' недопустимий діапазон значень балу бонітету [0..100] по типах культур."; err++;}
		}
		// Список не должен быть пустым
		if (i == 0)
			{report += "\n--- перелік полів не містить жодного запису."; err++;}
		if (err == 0) 
			report += "Ok.";
		
		sql.Execute("SELECT Poperedniki.ID, Poperedniki.Id_pop, Cultures.Name, Poperedniki.Vpliv FROM Cultures Left JOIN Poperedniki ON Cultures.ID = Poperedniki.Id_nast");
		report += "\nДовідник 'Попередники' ... ";
		err = 0; i = 0;
		double koef;
		while(sql.Fetch())
		{	
			koef = sql[3];
			if (IsNull(sql[3]) || IsNull(sql[0]))
				{report += "\n--- для культури '" + AsString(sql[2]) + "' не визначено вплив попередників."; err++;}
			if ((!IsNull(koef) && koef > 1) || (!IsNull(koef) && koef < 0))
				{report += "\n--- для культури '" + AsString(sql[2]) + "' недопустимий діапазон значень коефіцієнта впливу попередника [0..1]."; err++;}
			i++;
		}
		// Список не должен быть пустым
		if (i == 0)
			{report += "\n--- перелік попередників не містить жодного запису."; err++;}
		if (err == 0) 
			report += "Ok.";
		
		sql.Execute("SELECT Technologies.ID, Cultures.Name, Technologies.Urogai, Technologies.Cina, Technologies.Cost FROM Cultures INNER JOIN Technologies ON Cultures.ID = Technologies.Culture");
		report += "\nДовідник 'Технології' ... ";
		err = 0; i = 0;
		double income, urogai, cina, cost;
		while(sql.Fetch())
		{	
			urogai = sql[2];
			cina = sql[3];
			cost = sql[4];
			income = urogai * cina - cost;
			if (income < 0)
				{report += "\n--- для технології '" + AsString(sql[1]) + "' недопустимий діапазон значень показників прибутку [0...]."; err++;}
			i++;
		}
		// Список не должен быть пустым
		if (i == 0)
			{report += "\n--- перелік технологій не містить жодного запису."; err++;}
		if (err == 0) 
			report += "Ok.";
		
		// скільки часу зайняв розрахунок
		report += "\n\nЗавершено за " + tm.ToString() + " сек.";
		lnedit.SetData(report);	// виведення результату (звіту) у вікно
		lnedit.SetCursor(lnedit.GetLength()); // идем в конец отчета
	}
	
	// Функция веса ребра - чистый доход от технологии с учетом влияния поля и предшественника
	double getedgevalue(int parentID, int ID_tech, int ID_field, int ID_cult, int ID_popcult, int typecult, double area)
	{
		double urogainist = 0;
        int balfield = 0;
        int cultID;
        double koefpoperedink = 0;
        double cina = 0;
        double cost = 0;
        int rotationcount = 0;	// сколько лет не было этой культуры в пути
		bool ID_is_inpath; // есть ли вообще культура в пути
		
		// оценка требования ротации: считаем количество лет, прошедщих с последнего появления данной культуры на поле
		for (int i = GraphVertex[parentID].cultinpath.GetCount() - 1; i >= 0; i--)
		{
			if (GraphVertex[parentID].cultinpath[i] == ID_cult)
			{
				ID_is_inpath = true;
				break;
			}
			else 
				rotationcount++;;
		}
		
		// если культура есть в пути, то нужно проверить есть ли превышение нормы по ротации, не обращаем на требование ротации внимания
		if (ID_is_inpath)
		{
	        // если ротация по культуре превышена, возвращаем ноль
	        for (int i = 0; i < culttable.GetCount(); i++)
	        {
	            if (ID_cult == culttable[i].ID)
	            {
	                if (rotationcount < culttable[i].rotation)
	                {
	                	return 0;
	                }
	                break;
	            }
        	}
		}
		
		// оценка предшественника культуры
        for (int i = 0; i < popertable.GetCount(); i++)
        {
            if (ID_popcult == popertable[i].IDpop && ID_cult == popertable[i].IDnast)
            {
                koefpoperedink = popertable[i].vpliv;
                break;
            }
        }
        // если предшественник не подходит для культуры, возвращаем ноль
        if 	(koefpoperedink == 0)
			return 0;

		// оценка бала бонитета по культуре
        for (int i = 0; i < fieldbontable.GetCount(); i++)
        {
            if (ID_field == fieldbontable[i].field && typecult == fieldbontable[i].culturetype)
        	{
                balfield = fieldbontable[i].bal;
                break;
            }
        }
        // если почва не подходит и бонитет = 0  для этой культуры - возвращаем значение дохода ноль	
		if 	(balfield == 0)
			return 0;
		
		// Считаем прибыль с учетом влияния предшественника и бонитета почвы на урожайность. 
		// Урожайность корректируется на коэффициент, за который принимается либо бонитет либо предшественник, т.е. тот который из них меньше из них  
		// модель предполагает закон минимума факторов урожая
		// доход считается не с 1 га, а со всего поля
		
		for (int i = 0; i < techtable.GetCount(); i++)
        {
            if (ID_tech == techtable[i].ID)
        	{
                urogainist = techtable[i].Urogai;
                cina = techtable[i].Cina;
                cost = techtable[i].Cost;
                cultID = techtable[i].CultureID;
                break;
            }
        }
        double k = balfield;
        k = k / 100;
        if (koefpoperedink < k)
            k = koefpoperedink;
        double netvalue = (urogainist * k * cina - cost) * area;
        
        return netvalue;	
	}
	
	// Рекурсивная функция для построения графа переменных 
	void buildgraph (int parentID, int ID_field, int year, int ID_cult, double kumulvalue, double area)
    {
        if (year > Horizont)  // Сколько лет считать
             return;
        else
        { 
            // обход списка технологий
            for (int i = 0; i < techtable.GetCount(); i++)
            {
                int testculttype = culturetype(techtable[i].CultureID);
                double testedge = getedgevalue(parentID, techtable[i].ID, ID_field, techtable[i].CultureID, ID_cult, testculttype, area);
                // если доход по этой культуре больще нуля, то строим под ней следующую ветку, иначе эта культура не имеет пременной и остается без продолжения
                if (testedge > 0)
				{
					Myvertex& b = GraphVertex.Add();
					b.ID = n++;
					b.technologyID = techtable[i].ID;
					b.cultureID = techtable[i].CultureID;
					b.fieldID = ID_field;
					b.area = area;
					b.year = year;
					b.typecultureID = testculttype;
					for (int s = 0; s < GraphVertex[parentID].cultinpath.GetCount(); s++) // добавляются предыдущие культуры в список предшественников
						b.cultinpath.Add(GraphVertex[parentID].cultinpath[s]);
					b.cultinpath.Add(techtable[i].CultureID);	// добавляется текущая культура в список. Это нужно для оценки ротации культур в пути
					
					Myedge& c = GraphEdge.Add();
					c.PopVertexID = parentID;
					c.NastVertexID = b.ID;
					c.value = testedge;
					c.kumulvalue = kumulvalue + c.value;
					buildgraph(b.ID, ID_field, year + 1, b.cultureID, c.kumulvalue, area);
					Ctrl::ProcessEvents();					
				}
            }
        }
    }
		
	// Кнопка расчета севооборота
	void BtnRozrClick()
	{
		//EnterGuiMutex();
		TimeStop tm; // лічильник часу використаного на даний аналіз	
		lnedit.SetData("1. Дані ... ");
		
		// Обнуление графа
	    n = 0;
		GraphVertex.Clear();
        GraphEdge.Clear();
        techtable.Clear();
        fieldbontable.Clear();
		popertable.Clear();
		fieldtable.Clear();
		culttable.Clear();
		culttypetable.Clear();

		sql.Execute("SELECT * FROM CultureType");
		while(sql.Fetch())
		{
			Myculttypetable& t = culttypetable.Add();
			t.ID = sql[0];
			t.min = sql[2];
			t.max = sql[3];
		}

		sql.Execute("SELECT * FROM Cultures");
		while(sql.Fetch())
		{
			Myculttable& t = culttable.Add();
			t.ID = sql[0];
			t.type = sql[2];
			t.rotation = sql[3];
		}
		
		sql.Execute("SELECT * FROM Fields");
		while(sql.Fetch())
		{
			Myfieldtable& t = fieldtable.Add();
			t.ID = sql[0];
			t.area = sql[2];
			t.culture = sql[3];
		}
		
		sql.Execute("SELECT * FROM Technologies");
		while(sql.Fetch())
		{
			Mytechtable& t = techtable.Add();
			t.ID = sql[0];
			t.CultureID = sql[1];
			t.Urogai = sql[2];
			t.Cina = sql[3];
			t.Cost = sql[4];
		}
		
		sql.Execute("SELECT * FROM FieldsBonitet");
		while(sql.Fetch())
		{
			Myfieldbontable& t = fieldbontable.Add();
			t.ID = sql[0];
			t.field = sql[1];
			t.culturetype = sql[2];
			t.bal = sql[3];
		}
		
		sql.Execute("SELECT * FROM Poperedniki");
		while(sql.Fetch())
		{
			Mypopertable& t = popertable.Add();
			t.ID = sql[0];
			t.IDpop = sql[1];
			t.IDnast = sql[2];
			t.vpliv = sql[3];
		}
		lnedit.SetData(lnedit.Get() + "Ok.\n2. Граф змінних ... ");
		Ctrl::ProcessEvents();
		
		// Обход каждого поля: построение графа с рекурсией
		for (int i = 0; i < fieldtable.GetCount(); i++)
		{	
			Myvertex& d = GraphVertex.Add();
			d.ID = n++;
			d.cultureID = fieldtable[i].culture;
			d.fieldID = fieldtable[i].ID;
			d.year = 0;
			d.technologyID = 0;
			d.typecultureID = culturetype(fieldtable[i].culture);
			d.cultinpath.Add(fieldtable[i].culture);
			buildgraph(d.ID, d.fieldID, 1, d.cultureID, 0, fieldtable[i].area);
		}	
		
		// если нет ребер, значит не должно быть и вершин в графе (удаляем все вершины поскольку, не удалось создать граф переменных, модели тоже не будет)
		if (GraphEdge.GetCount() == 0)
			GraphVertex.Clear();
	
		/*
		String cultsinpath;
		FileOut out3("cultinpath.csv");
		if(!out3) {
			Exclamation("Unable to open cultinpath.csv");
		}
		else
		{
			for (int i = 0; i < GraphVertex.GetCount(); i++)
			{
				cultsinpath = "";
				for (int c = 0; c < GraphVertex[i].cultinpath.GetCount(); c++)
					cultsinpath += AsString(GraphVertex[i].cultinpath[c]) + ";";
			out3.PutLine(AsString(GraphVertex[i].ID) + ";" + cultsinpath);
			}
		}
		*/
		lnedit.SetData(lnedit.Get() + "Ok.\n---- Кількість вузлів - " + AsString(GraphVertex.GetCount()) +".");
		lnedit.SetData(lnedit.Get() + "\n3. Модель оптимізації ... ");
		Ctrl::ProcessEvents();
		
		//================================================  МОДЕЛЬ ОПТИМИЗАЦИИ
		lprec *lp;
		int Ncol, *colno = NULL, j, ret = 0;
	  	REAL *row = NULL;
	 	
	  	// We will build the model row by row
	    //So we start with creating a model with 0 rows and 2 columns 
	  	Ncol = GraphVertex.GetCount();
	  	lp = make_lp(0, Ncol);
	  
	  	if(lp == NULL)
	  	{
	    	ret = 1; // couldn't construct a new model... 
	    	lnedit.SetData(lnedit.Get() + "\n---- Помилка створення моделі. ");
	  	}
	  	if(ret == 0) {
	      	// create space large enough for one row 
	    	colno = (int *) malloc(Ncol * sizeof(*colno));
	    	row = (REAL *) malloc(Ncol * sizeof(*row));
	    	if((colno == NULL) || (row == NULL))
	      	ret = 2;
  		}
		
		if(ret == 0) {
			
			set_add_rowmode(lp, TRUE);  // makes building the model faster if it is done rows by row 
  			
  			Vector < Vector <int> > array;
  			
  			// Ограничения по начальным полям в нулевом году
			for (int c = 0; c < GraphVertex.GetCount(); c++)
			{
				set_binary(lp, GraphVertex[c].ID + 1, TRUE); // sets all variables to binary
				array.Add(); // создаем контейнер для ограничений по площадям
				if (GraphVertex[c].year == 0)
				{
					j = 0;
					colno[j] = GraphVertex[c].ID + 1;
					row[j++] = 1;
					add_constraintex(lp, j, row, colno, EQ, 1);
				}
			}

			// Ограничение по площади вариантов под каждым узлом
			for (int c = 0; c < GraphEdge.GetCount(); c++)
			{
				array[GraphEdge[c].PopVertexID].Add(GraphEdge[c].NastVertexID);
			}
			
			for (int c = 0; c < array.GetCount(); c++)
			{
				if (array[c].GetCount() > 0)
				{
					j = 0;
					colno[j] = c + 1;
					row[j++] = 1;
					for (int u = 0; u < array[c].GetCount(); u++)
					{
						colno[j] = array[c][u] + 1;
						row[j++] = -1;
					}
					add_constraintex(lp, j, row, colno, GE, 0);
				}
			}
						
			// Ограничения по минимуму и максимуму культур по типам в каждом из пяти лет
			double total_area = 0; // загальна площа всіх полів
			for (int i = 0; i < fieldtable.GetCount(); i++)
			{
				total_area += fieldtable[i].area;
			}
			int maxtypeID = 0; // максимальне значення ID в таблиці типів культур
			for (int i = 0; i < culttypetable.GetCount(); i++)
			{
				if (maxtypeID < culttypetable[i].ID)
					maxtypeID = culttypetable[i].ID;
			}
			
			Vector < Vector < Vector <Myvar> > > var_array; // [рік][код типу культур][счетчик] MyVar	
			
			for (int c_year = 0; c_year <= Horizont; c_year++) //  - кількість років
			{
				var_array.Add();
				for (int n = 0; n <= maxtypeID; n++) // індекс типів культур
				{
					var_array[c_year].Add();
				}
			}			
			
			int columnID;
			for (int i = 0; i < GraphVertex.GetCount(); i++)
			{ 
				columnID = GraphVertex[i].ID + 1;
				Myvar& b = var_array[GraphVertex[i].year][GraphVertex[i].typecultureID].Add();
				b.column = columnID;
				b.area = GraphVertex[i].area;
			}
					
			/*
			FileOut out4("culttype.csv");
			if(!out4) {
				Exclamation("Unable to open culttype.csv");
			}
			else
			{
				String text;
				for (int c_year = 0; c_year <= Horizont; c_year++) // - кількість років
				{
					out4.PutLine("Рік - " + AsString(c_year));
					for (int n = 0; n <= maxtypeID; n++) // індекс типів культур
					{
						text = "";
						out4.PutLine("Тип культури - " + AsString(n));
						for (int u = 0; u < var_array[c_year][n].GetCount(); u++)
							text += AsString(var_array[c_year][n][u].column) + "; ";
						out4.PutLine(text);
					}
				}				
			}
			*/
			
			double minvalue, maxvalue;
			for (int c_year = 1; c_year <= Horizont; c_year++) // - кількість років
			{
				for (int n = 0; n <= maxtypeID; n++) // індекс типів культур
				{
					if (var_array[c_year][n].GetCount() > 0)
					{
						j = 0;
						for (int u = 0; u < var_array[c_year][n].GetCount(); u++)
						{
							colno[j] = var_array[c_year][n][u].column;
							row[j++] = var_array[c_year][n][u].area;
						}
						
						for (int i = 0; i < culttypetable.GetCount(); i++)
						{
							if (culttypetable[i].ID == n)
							{
								minvalue = culttypetable[i].min * total_area / 100;
								maxvalue = culttypetable[i].max * total_area / 100;
								add_constraintex(lp, j, row, colno, GE, minvalue);
								add_constraintex(lp, j, row, colno, LE, maxvalue);
								break;
							}
						}
					}	
				}
			}
		} 	
		
		if(ret == 0) {
			set_add_rowmode(lp, FALSE); /* rowmode should be turned off again when done building the model
				
		    /* set the objective function */
		    j = 0;
			
			for (int c = 0; c < GraphEdge.GetCount(); c++)
			{
				colno[j] = GraphEdge[c].NastVertexID + 1;
				row[j++] = GraphEdge[c].value;
			}
			
		    /* set the objective in lpsolve */
		    if(!set_obj_fnex(lp, j, row, colno))
		      ret = 4;
		}

		if(ret == 0) {
		    /* set the object direction to maximize */
		    set_maxim(lp);
			
		    /* just out of curioucity, now show the model in lp format on screen */
		    //set_outputfile(lp, "model.lp");
		    String AppPath = GetFileDirectory(GetExeFilePath());
		    AppPath << "model.lp";
		    
		    // Конвертація шляху до програми у формат char*
		    StringBuffer sb = AppPath;
			char *s = ~sb;
			
		    //write_lp(lp, "model.lp")
		    if (optSaveGrapf.GetData())
		   	{
		    	write_lp(lp, s);
		   	}
			
			/* I only want to see important messages on screen while solving */
		    set_verbose(lp, IMPORTANT);
			
		    /* Now let lpsolve calculate a solution */
		    lnedit.SetData(lnedit.Get() + "\n---- Модель побудовано. Калькуляція ...");
		    Ctrl::ProcessEvents();
		    //установим время таймаута в сек.
		    set_timeout(lp, 960);
		    ret = solve(lp);
		    
		    switch (ret)
		    {
			    case 0:
			    {
			    	lnedit.SetData(lnedit.Get() + "\n---- Знайдено оптимальне рішення моделі.");
			    	break;
			    }
			   	case -2:
			   	{
			    	lnedit.SetData(lnedit.Get() + "\n---- Помилка: переповнення пам`яті.");
			    	break;
			    }
			   	case 1:
			   	{
			    	lnedit.SetData(lnedit.Get() + "\n---- Розв`язок моделі є частково оптимальним.");
			   		break;
			    }
			    case 2:
			    {
			    	lnedit.SetData(lnedit.Get() + "\n---- Помилка: модель є неприпустимою.");
			   		break;
			    }
			   	case 3:
			   	{
			    	lnedit.SetData(lnedit.Get() + "\n---- Помилка: модель необмежена.");
			   		break;
			    }
			   	case 4:
			   	{
			    	lnedit.SetData(lnedit.Get() + "\n---- Помилка: модель невірна.");
			   		break;
			    }
			   	case 5:
			   	{
			    	lnedit.SetData(lnedit.Get() + "\n---- Помилка: не вдалось розрахувати.");
			   		break;
			    }
			   	case 6:
			   	{
			    	lnedit.SetData(lnedit.Get() + "\n---- Помилка: розрахунок було перервано.");
					break;
			    }
				case 7:
				{
			    	lnedit.SetData(lnedit.Get() + "\n---- Помилка: завершився час.");
			    	break;
			    }
			    case 9:
			    {
			    	lnedit.SetData(lnedit.Get() + "\n---- Помилка: The model could be solved by presolve.");
			    	break;
			    }
			    case 10:
			    {
			    	lnedit.SetData(lnedit.Get() + "\n---- Помилка: The B&B routine failed.");		  
			    	break;
			    }
			    case 11:
			    {
			    	lnedit.SetData(lnedit.Get() + "\n---- Помилка: The B&B was stopped because of a break-at-first or a break-at-value."); 
			    	break;
			    }
			    case 12:
			    {
			    	lnedit.SetData(lnedit.Get() + "\n---- Знайдено ймовірне рішення.");	
			    	break;
			    }
			    case 13:
			    {
			    	lnedit.SetData(lnedit.Get() + "\n---- Не знайдено ймовірного рішення.");
		    		break;
			    }
		    }
		    if(ret == OPTIMAL)
		    	ret = 0;
		    else
		    	ret = 5;
		}

		if(ret == 0) {
		    /* a solution is calculated, now lets get some results */
		    /* objective value */
		    //printf("Objective value: %f\n", get_objective(lp));
			//PromptOK(Format("%f", get_objective(lp)));

		    lnedit.SetData(lnedit.Get() + "\n---- Цільова функція - " + Format("%.2f", get_objective(lp)) + " грн.");
		    get_variables(lp, row);
		   	for(j = 0; j < Ncol; j++)
		    	GraphVertex[j].variable = int(row[j]);
		   	
		   	sql.Execute("UPDATE Fields SET Year1 = Null");
		   	sql.Execute("UPDATE Fields SET Year2 = Null");
		   	sql.Execute("UPDATE Fields SET Year3 = Null");
		   	sql.Execute("UPDATE Fields SET Year4 = Null");
		   	sql.Execute("UPDATE Fields SET Year5 = Null");
		   	sql.Execute("UPDATE Fields SET Year6 = Null");
		   	sql.Execute("UPDATE Fields SET Year7 = Null");
		   	sql.Execute("UPDATE Fields SET Year8 = Null");
		   	sql.Execute("UPDATE Fields SET Year9 = Null");
		   	sql.Execute("UPDATE Fields SET Year10 = Null");
		   	
		   	for (int i = 0; i < GraphVertex.GetCount(); i++)
		   	{
		   		if (GraphVertex[i].variable == 1)
		   		{
		   			if(GraphVertex[i].year == 1)
		   				sql.Execute("UPDATE Fields SET Year1 = '" + AsString(GraphVertex[i].technologyID) + " ' WHERE ID = " + AsString(GraphVertex[i].fieldID));
		   			if(GraphVertex[i].year == 2)
		   				sql.Execute("UPDATE Fields SET Year2 = '" + AsString(GraphVertex[i].technologyID) + " ' WHERE ID = " + AsString(GraphVertex[i].fieldID));
		   			if(GraphVertex[i].year == 3)
		   				sql.Execute("UPDATE Fields SET Year3 = '" + AsString(GraphVertex[i].technologyID) + " ' WHERE ID = " + AsString(GraphVertex[i].fieldID));
		   			if(GraphVertex[i].year == 4)
		   				sql.Execute("UPDATE Fields SET Year4 = '" + AsString(GraphVertex[i].technologyID) + " ' WHERE ID = " + AsString(GraphVertex[i].fieldID));
		   			if(GraphVertex[i].year == 5)
		   				sql.Execute("UPDATE Fields SET Year5 = '" + AsString(GraphVertex[i].technologyID) + " ' WHERE ID = " + AsString(GraphVertex[i].fieldID));
		   			if(GraphVertex[i].year == 6)
		   				sql.Execute("UPDATE Fields SET Year6 = '" + AsString(GraphVertex[i].technologyID) + " ' WHERE ID = " + AsString(GraphVertex[i].fieldID));
		   			if(GraphVertex[i].year == 7)
		   				sql.Execute("UPDATE Fields SET Year7 = '" + AsString(GraphVertex[i].technologyID) + " ' WHERE ID = " + AsString(GraphVertex[i].fieldID));
		   			if(GraphVertex[i].year == 8)
		   				sql.Execute("UPDATE Fields SET Year8 = '" + AsString(GraphVertex[i].technologyID) + " ' WHERE ID = " + AsString(GraphVertex[i].fieldID));
		   			if(GraphVertex[i].year == 9)
		   				sql.Execute("UPDATE Fields SET Year9 = '" + AsString(GraphVertex[i].technologyID) + " ' WHERE ID = " + AsString(GraphVertex[i].fieldID));
		   			if(GraphVertex[i].year == 10)
		   				sql.Execute("UPDATE Fields SET Year10 = '" + AsString(GraphVertex[i].technologyID) + " ' WHERE ID = " + AsString(GraphVertex[i].fieldID));
		   		}
		   	}
		   	
		   	if (optSaveGrapf.GetData())
		   	{
	   			FileOut out("nodes.csv");
				if(!out) {
					Exclamation("Unable to open nodes.csv");
				}
				else
				{
					out.PutLine("Id;Label;FieldID;Year;TechID;CultType;Variable");
					for (int i = 0; i < GraphVertex.GetCount(); i++)
						out.PutLine(AsString(GraphVertex[i].ID) + ";" + AsString(GraphVertex[i].cultureID) + ";" + AsString(GraphVertex[i].fieldID) + ";" + AsString(GraphVertex[i].year) + ";" + AsString(GraphVertex[i].technologyID) + ";" + AsString(GraphVertex[i].typecultureID)  + ";" + AsString(GraphVertex[i].variable));
				}	   	
			   	
			   	FileOut out2("edges.csv");
				if(!out2) {
					Exclamation("Unable to open edges.csv");
				}
				else
				{
					out2.PutLine("Source;Target;Type;Id;Label;Weight");
					for (int i = 0; i < GraphEdge.GetCount(); i++)
						out2.PutLine(AsString(GraphEdge[i].PopVertexID) + ";" + AsString(GraphEdge[i].NastVertexID) + ";Directed;" + AsString(i) + ";" + AsString(i) + ";" + AsString(GraphEdge[i].kumulvalue));
				}
		   	}
		   	
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
		
		/* free allocated memory */
		if(row != NULL)
		    free(row);
		if(colno != NULL)
		    free(colno);
		/* clean up such that all used memory by lpsolve is freed */
		if(lp != NULL) 
		    delete_lp(lp);
		
		lnedit.SetData(lnedit.Get() + "\n\nЗавершено за " + tm.ToString() + " сек.");
		btnRozr.Enable(true);
		btnData.Enable(true);
	}
	
	void Update()
	{
		try
		{
			String sqlrequest;
			sql.Execute("SELECT * FROM [" + TableName + "] LIMIT 1");
			//for(int i = 1; i < sql.GetColumns() - 1 ; i++)
			for(int i = 1; i < Horizont + 4 ; i++)
			{	
				sqlrequest += sql.GetColumnInfo(i).name + "='" + AsString(grid(i)) + "' ";
				if(i < Horizont + 3)
				 	sqlrequest +=", ";
			}
			sql.Execute("UPDATE [" + TableName + "] SET " + sqlrequest + "WHERE ID='" + AsString(grid(0)) + "'");
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
		// удаление имеющихся и вывод нужных стобцов
		grid.Clear();
		grid.RemoveColumn(0, grid.GetColumnCount()); // удаляем все имеющиеся стобцы
		grid.AddIndex(ID);
		grid.AddColumn("Назва (номер) поля", 110).Edit(edName);
		grid.AddColumn("Площа поля, га", 90).Edit(edArea).AlignRight().SetConvert(Single<ConvDouble>());
		grid.AddColumn("Поточна культура", 120).Edit(edCurCulture).SetConvert(Single<CultConvert>());
		if (Horizont >= 1)
			grid.AddColumn("", 160).Edit(edTech1).SetConvert(Single<TechCultConvert>());
		if (Horizont >= 2)
			grid.AddColumn("", 160).Edit(edTech2).SetConvert(Single<TechCultConvert>());
		if (Horizont >= 3)
			grid.AddColumn("", 160).Edit(edTech3).SetConvert(Single<TechCultConvert>());
		if (Horizont >= 4)
			grid.AddColumn("", 160).Edit(edTech4).SetConvert(Single<TechCultConvert>());
		if (Horizont >= 5)
			grid.AddColumn("", 160).Edit(edTech5).SetConvert(Single<TechCultConvert>());
		if (Horizont >= 6)
			grid.AddColumn("", 160).Edit(edTech6).SetConvert(Single<TechCultConvert>());
		if (Horizont >= 7)
			grid.AddColumn("", 160).Edit(edTech7).SetConvert(Single<TechCultConvert>());
		if (Horizont >= 8)
			grid.AddColumn("", 160).Edit(edTech8).SetConvert(Single<TechCultConvert>());
		if (Horizont >= 9)
			grid.AddColumn("", 160).Edit(edTech9).SetConvert(Single<TechCultConvert>());
		if (Horizont >= 10)
			grid.AddColumn("", 160).Edit(edTech10).SetConvert(Single<TechCultConvert>());
		// установим заголовки лет в таблице
		int cyear;
		sql.Execute("SELECT * FROM Project WHERE Param = 'Year'");
		if(sql.Fetch())
			cyear = sql[1];
		for (int i = 0; i <= Horizont; i++)
			grid.GetColumn(3 + i).Name(AsString(cyear + i));
		grid.RefreshTop();
		// заполнение выпадающего списка текущей культуры
		edCurCulture.Clear();
		sql.Execute("SELECT * FROM Cultures ORDER BY Name");
		while(sql.Fetch())
			edCurCulture.Add(sql[0],sql[1]);
		// заполнение выпадающих списков технологий
		edTech1.Clear();
		edTech2.Clear();
		edTech3.Clear();
		edTech4.Clear();
		edTech5.Clear();
		edTech6.Clear();
		edTech7.Clear();
		edTech8.Clear();
		edTech9.Clear();
		edTech10.Clear();
		String text; 
		sql.Execute("SELECT Technologies.ID, Cultures.Name, Technologies.Urogai FROM Cultures INNER JOIN Technologies ON Cultures.ID = Technologies.Culture");
		while(sql.Fetch())
		{
			text=sql[1]; 
			text<<", ";
			text<<sql[2];
			text<<" т/га";
			edTech1.Add(sql[0],text);
			edTech2.Add(sql[0],text);
			edTech3.Add(sql[0],text);
			edTech4.Add(sql[0],text);
			edTech5.Add(sql[0],text);
			edTech6.Add(sql[0],text);
			edTech7.Add(sql[0],text);
			edTech8.Add(sql[0],text);
			edTech9.Add(sql[0],text);
			edTech10.Add(sql[0],text);
		}
		// заполнение таблицы
		//grid.Clear();
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

	CoWork thread1;
	
	void run_thread1()
	{
		thread1.Do(THISBACK(BtnRozrClick));
		btnData.Enable(false);
		btnRozr.Enable(false);
	}
	
	volatile Atomic terminated1;

    typedef PanSiv CLASSNAME;
 	PanSiv()
 	{
 		CtrlLayout(*this);
 		CtrlLayout(mainctrl);
 		
 		mainctrl.Add(btnData.LeftPosZ(5, 90).TopPosZ(15, 28));
 		btnData.SetLabel("Аналіз");
 		mainctrl.Add(btnRozr.LeftPosZ(100, 90).TopPosZ(15, 28));
 		btnRozr.SetLabel("Розрахунок");	
 		mainctrl.Add(optSaveGrapf.LeftPosZ(210, 150).TopPosZ(15, 28));
 		optSaveGrapf.SetLabel("Зберегти модель в файл");
 		mainctrl.Add(lnedit.HSizePosZ(0, 5).VSizePosZ(55, 5));
 		split.Vert(grid, mainctrl);
 		split.SetPos(6500);
 		Add(split.SizePos());
 		TableName = "Fields";
 		
 		/*
		grid.AddIndex(ID);
		
		grid.AddColumn("Назва (номер) поля", 110).Edit(edName);
		grid.AddColumn("Площа поля, га", 90).Edit(edArea).AlignRight().SetConvert(Single<ConvDouble>());
		grid.AddColumn("Поточна культура", 120).Edit(edCurCulture).SetConvert(Single<CultConvert>());
		grid.AddColumn("", 160).Edit(edTech1).SetConvert(Single<TechCultConvert>());
		grid.AddColumn("", 160).Edit(edTech2).SetConvert(Single<TechCultConvert>());
		grid.AddColumn("", 160).Edit(edTech3).SetConvert(Single<TechCultConvert>());
		grid.AddColumn("", 160).Edit(edTech4).SetConvert(Single<TechCultConvert>());
		grid.AddColumn("", 160).Edit(edTech5).SetConvert(Single<TechCultConvert>());
		grid.AddColumn("", 160).Edit(edTech6).SetConvert(Single<TechCultConvert>());
		grid.AddColumn("", 160).Edit(edTech7).SetConvert(Single<TechCultConvert>());
		grid.AddColumn("", 160).Edit(edTech8).SetConvert(Single<TechCultConvert>());
		grid.AddColumn("", 160).Edit(edTech9).SetConvert(Single<TechCultConvert>());
		grid.AddColumn("", 160).Edit(edTech10).SetConvert(Single<TechCultConvert>());
		*/
		grid.WhenUpdateRow = THISBACK(Update);
		grid.Editing().Accepting().Canceling().Indicator().Searching().Absolute().SetToolBar(); //Proportional()
		
		// Таблиця випадаючого списку поточної культури					
		edCurCulture.AddColumn("ID").Width(0);
		edCurCulture.AddColumn("Поточна культура", 110);
		edCurCulture.SetKeyColumn(0);
		edCurCulture.SetValueColumn(1);
		
		// Таблиця випадаючого списку технологій перщого року					
		edTech1.AddColumn("ID").Width(0);
		edTech1.AddColumn("Технологія", 110);
		edTech1.SetKeyColumn(0);
		edTech1.SetValueColumn(1);
		
		// Таблиця випадаючого списку технологій другого року					
		edTech2.AddColumn("ID").Width(0);
		edTech2.AddColumn("Технологія", 110);
		edTech2.SetKeyColumn(0);
		edTech2.SetValueColumn(1);
		
		// Таблиця випадаючого списку технологій третього року					
		edTech3.AddColumn("ID").Width(0);
		edTech3.AddColumn("Технологія", 110);
		edTech3.SetKeyColumn(0);
		edTech3.SetValueColumn(1);
		
		// Таблиця випадаючого списку технологій четвертого року					
		edTech4.AddColumn("ID").Width(0);
		edTech4.AddColumn("Технологія", 110);
		edTech4.SetKeyColumn(0);
		edTech4.SetValueColumn(1);
		
		// Таблиця випадаючого списку технологій пятого року					
		edTech5.AddColumn("ID").Width(0);
		edTech5.AddColumn("Технологія", 110);
		edTech5.SetKeyColumn(0);
		edTech5.SetValueColumn(1);
		
		// Таблиця випадаючого списку технологій 					
		edTech6.AddColumn("ID").Width(0);
		edTech6.AddColumn("Технологія", 110);
		edTech6.SetKeyColumn(0);
		edTech6.SetValueColumn(1);

		// Таблиця випадаючого списку технологій пятого року					
		edTech7.AddColumn("ID").Width(0);
		edTech7.AddColumn("Технологія", 110);
		edTech7.SetKeyColumn(0);
		edTech7.SetValueColumn(1);
		
		// Таблиця випадаючого списку технологій пятого року					
		edTech8.AddColumn("ID").Width(0);
		edTech8.AddColumn("Технологія", 110);
		edTech8.SetKeyColumn(0);
		edTech8.SetValueColumn(1);
		
		// Таблиця випадаючого списку технологій пятого року					
		edTech9.AddColumn("ID").Width(0);
		edTech9.AddColumn("Технологія", 110);
		edTech9.SetKeyColumn(0);
		edTech9.SetValueColumn(1);
		
		// Таблиця випадаючого списку технологій пятого року					
		edTech10.AddColumn("ID").Width(0);
		edTech10.AddColumn("Технологія", 110);
		edTech10.SetKeyColumn(0);
		edTech10.SetValueColumn(1);
		
		lnedit.SetFont(Monospace());
		
		btnRozr.WhenAction = THISBACK(run_thread1);
		btnData.WhenAction = THISBACK(BtnDataClick);
	};
};

#endif
