// Strukturierte Listen.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//Informatik 1
//Strukturierte Listen
//Lennart Theiler
//08.12.2023

#include <iostream>
using namespace std;


struct Student
{
    string Vorname;
    string Nachname;
    int Alter;
};

void Ausgabe(int fest_student, int num_student, Student studentListe[], int interval_Feld[])
{
    for (int i = 0; i < fest_student + num_student; i++)
    {
        cout << "Studierender #" << i + 1 << ": "
            << studentListe[i].Vorname << ", "
            << studentListe[i].Nachname << ", "
            << studentListe[i].Alter << "."
            << endl;
    }
    cout << endl;

    for (int i = 0; i < 10; i++)
    {
        if (i<9)
        {

            cout << i << "1" << " bis " << i + 1 << "0:   " << interval_Feld[i] << endl;
        }
        else
        {

            cout << i << "1" << " bis " << i + 1 << "0:  " << interval_Feld[i] << endl;
        }
    }
}

void Zusammenfassung(int fest_student, int num_student, Student studentListe[])
{
    int interval1_10 = 0, interval11_20 = 0, interval21_30 = 0, interval31_40 = 0, interval41_50 = 0, interval51_60 = 0, interval61_70 = 0, interval71_80 = 0, interval81_90 = 0, interval91_99 = 0;

    for (int i = 0; i < fest_student + num_student; i++)
    {
        if (studentListe[i].Alter >= 1 && studentListe[i].Alter < 11) { interval1_10++; }
        else if (studentListe[i].Alter >= 11 && studentListe[i].Alter <=20) { interval11_20++; }
        else if (studentListe[i].Alter >= 21 && studentListe[i].Alter <=30) { interval21_30++; }
        else if (studentListe[i].Alter >= 31 && studentListe[i].Alter <=40) { interval31_40++; }
        else if (studentListe[i].Alter >= 41 && studentListe[i].Alter <=50) { interval41_50++; }
        else if (studentListe[i].Alter >= 51 && studentListe[i].Alter <=60) { interval51_60++; }
        else if (studentListe[i].Alter >= 61 && studentListe[i].Alter <=70) { interval61_70++; }
        else if (studentListe[i].Alter >= 71 && studentListe[i].Alter <=80) { interval71_80++; }
        else if (studentListe[i].Alter >= 81 && studentListe[i].Alter <=90) { interval81_90++; }
        else if (studentListe[i].Alter >= 91 && studentListe[i].Alter <100) { interval91_99++; }
    }
    int interval_Feld[10] = { interval1_10, interval11_20, interval21_30, interval31_40, interval41_50, interval51_60, interval61_70, interval71_80, interval81_90, interval91_99 };
    
    Ausgabe(fest_student, num_student, studentListe, interval_Feld);

}



void BubbleSort(Student studentListe[], int num_student, int fest_student)
{
    for (int i = 0; i < num_student + fest_student ; i++)
    {
        for (int j = 0; j < num_student + fest_student - 1; j++)
        {
            if (studentListe[j].Nachname > studentListe[j + 1].Nachname)
            {
                Student swap = studentListe[j];
                studentListe[j] = studentListe[j + 1];
                studentListe[j + 1] = swap;

            }
        }
    }
    Zusammenfassung(fest_student, num_student, studentListe);
}

void automatischAngelegt(int num_student, int fest_student)
{
    Student* studentListe = new Student[5+num_student];

    
    Student temp1;
    temp1.Vorname = "Peter";
    temp1.Nachname = "Abel";
    temp1.Alter = 42;

    studentListe[0] = temp1;
    
    temp1.Vorname = "Bugs";
    temp1.Nachname = "Bunny";
    temp1.Alter = 47;

    studentListe[1] = temp1;

    temp1.Vorname = "Donald";
    temp1.Nachname = "Duck";
    temp1.Alter = 64;

    studentListe[2] = temp1;

    temp1.Vorname = "Mickey";
    temp1.Nachname = "Mouse";
    temp1.Alter = 61;

    studentListe[3] = temp1;

    temp1.Vorname = "Anton";
    temp1.Nachname = "Zabel";
    temp1.Alter = 23;

    studentListe[4] = temp1;

    for (int i = 0; i < num_student; i++)
    {
        Student temp;
        cout << endl << "Erfassung von Studierenden Nr." << i+1 << endl;
        cout << "Vorname: "; 
        cin >> temp.Vorname;
        cout << "Nachname: "; 
        cin >> temp.Nachname;
        cout << "Alter: "; 
        cin >> temp.Alter;
        while (temp.Alter < 1 || temp.Alter > 99)
        {
            cout << "Fehler! Korrigiere deine Eingabe!\n";
            cout << "Alter: ";
            cin >> temp.Alter;
        }

        studentListe[i+5] = temp;
    }



    BubbleSort(studentListe, num_student, fest_student);
}


int main()
{
    int fest_student = 5;
    int num_student = 0;
    cout << "Geben Sie die Anzahl der Studenten ein, die sie inzialisieren wollen > ";
    cin >> num_student;
    automatischAngelegt(num_student, fest_student);
}
