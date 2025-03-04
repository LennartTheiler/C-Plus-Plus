// Informatik 1
// Bildverarbeitung
// Autor: Lennart Theiler

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
using namespace std;

struct BmpMetaData
{
	// durch Datei-Lesen gesetzt
	unsigned char* Data;
	unsigned int FileLen = 0;

	// durch DecodeHeader zu setzen
	unsigned short FileType;		// 2 Bytes an Bytepos = 0, müssen 0x4d42 sein
	unsigned int PosPixelData;		// 4 Bytes an Bytepos = 10
	unsigned int Width;				// 4 Bytes an Bytepos = 18
	unsigned int Height;			// 4 Bytes an Bytepos = 22
	unsigned short BitCount;		// 2 Bytes an Bytepos = 28
	unsigned int PaletteCount;		// 4 Bytes an Bytepos = 46
};

bool ReadFile(string fn, BmpMetaData& img)
{
	// Datei im Stil "C" oeffnen
	FILE* file = fopen(fn.c_str(), "rb");
	if (file == NULL)
		return false;

	// Dateigroesse erfragen
	fseek(file, 0, SEEK_END);
	img.FileLen = ftell(file);
	fseek(file, 0, SEEK_SET);

	// Datenpuffer holen
	img.Data = new unsigned char[img.FileLen];

	// Daten lesen und Datei schliessen (!)
	fread(img.Data, img.FileLen, 1, file);
	fclose(file);

	// OK
	return true;
}

BmpMetaData DuplicateImage(BmpMetaData& img)
{
	BmpMetaData res = img;
	res.Data = new unsigned char[res.FileLen];
	memcpy(res.Data, img.Data, img.FileLen);
	return res;
}

bool WriteFile(string fn, BmpMetaData& img)
{
	// Datei im Stil "C" oeffnen
	FILE* file = fopen(fn.c_str(), "wb");
	if (file == NULL)
		return false;

	// Daten schreiben und Datei schliessen (!)
	fwrite(img.Data, img.FileLen, 1, file);
	fclose(file);

	// OK
	return true;
}



//---------------------------------------------------------------------------------------------------------------------------------------------
//						dekodierung der Daten die im BMP Format gespeichert sind, um mit diesen weiter arbeiten zu können
//---------------------------------------------------------------------------------------------------------------------------------------------
unsigned unsigned short Decode2Bytes(BmpMetaData& img, int pos)
{
	return img.Data[pos] + 256 * img.Data[pos + 1];

	// 1. img.Data[pos]							-->	Zugriff aus die Daten
	// 2. img.Data[pos+1]						-->	Zugriff auf das nächste/folgende Byte
	// 3. img.Data[pos] +256 * img.Data[pos+1]	-->	Kombination der Bytes. Um diese kombinieren zu können müssen sie äquivalent sein.
	//												Erreichen kann man das, wenn man das höherwertige Byte mit 2^8 (256) multipliziert.
	// 4. Rückgabe wert							--> Das kombinierte Byte wird als unsigned short zurückgegeben.
}

unsigned unsigned int Decode4Bytes(BmpMetaData& img, int pos)
{
	return img.Data[pos]
		| (img.Data[pos + 1] << 8)			//	-->	Erstnächstes Byte muss um 8 Bit verschoben werden	Rechenoperation: *2^8
		| (img.Data[pos + 2] << 16)			//	-->	Zweites Byte muss um 16 Bit verschoben werden		Rechenoperation: *2^16
		| (img.Data[pos + 3] << 24);		//	-->	drittes Byte muss um 24 Bit verschoben werden		Rechenoperation: *2^24
}
//---------------------------------------------------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------------------------------------------------
//										Aufruf der Dekodierungsfunktionen um BMP zu dekodieren
//---------------------------------------------------------------------------------------------------------------------------------------------
bool DecodeHeader(BmpMetaData& img)
{
	img.FileType = Decode2Bytes(img, 0);
	img.PosPixelData = Decode4Bytes(img, 10);
	img.Width = Decode4Bytes(img, 18);
	img.Height = Decode4Bytes(img, 22);
	img.BitCount = Decode2Bytes(img, 28);
	img.PaletteCount = Decode4Bytes(img, 46);

	if (img.FileType == 0x4d42 && img.BitCount == 8)
	{
		return true;
	}
	else
	{
		return 0;
	}

	// 1. img.FileType = Decode2Bytes(img, 0)		-->	Hier werden die ersten zwei Bytes der Datei dekodiert. Die ersten zwei Bytes geben den Dateityp an	|
	// 2. img.PosPixelData = Decode4Bytes(img, 10)	--> Mit dem Aufruf der Decode4Bytes Funktion werden hier die Pixelpositionen dekodiert					|
	// 3. img.Width = Decode4Bytes(img, 18);		--> durch diesen Part wird die Breite und Höhe des Bildes dekodiert										|
	//    img.Height = Decode4Bytes(img, 22);																												 > Werden in den entsprechenden Feldern Gespeichert, die in den Klammern angegeben sind
	// 4. img.BitCount = Decode2Bytes(img, 28);		--> Durch diese Zeile wird die Bitanzahl dekodiert														|
	// 5. img.PaletteCount = Decode4Bytes(img, 46);	--> Hier werden die Einträge in der Farbpalette dekodiert												|
	// 6. FileType == 0x4d42						--> 0x4d42 steht in der ASCII-Tabelle für "BM", was das Kennzeichen einer BMP-Datei ist
	// 7. if (FileType == 0x4d42 && BitCount == 8)	--> Ist es True, wird True weitergegeben und die Bildverarbeitung geht in den nächsten Schritt
}
//---------------------------------------------------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------------------------------------------------
//												Überprüfung des Bildes und Setzen der Pixel
//---------------------------------------------------------------------------------------------------------------------------------------------
unsigned char GetPixel(BmpMetaData& img, int x, int y)
{
	if (x < 0 || x >= img.Width || y < 0 || y >= img.Height)
	{
		return 0;

		// Wenn X kleiner als 0 ist oder größer als die Breite des Bildes 
		// oder Y kleiner als 0 ist oder größer als die Höhe des Bildes 
		// wird das Programm nicht weiter fortgeführt
	}
	else
	{
		int index = img.PosPixelData + x + y * img.Width;
		return (img.Data[index]);

		// Sind Die X und Y Koordinaten gültig so wird der Index im Datenarray berechnet. Der Index ergint sich aus der Startposition der Pixel-Daten,
		// der horizontalen Position (X) und der vertikalen Position (Y). Die vertikale Position des Pixels wird mit der Breite des Bildes multipliziert,
		// um die Position im eindimensionalen Array korrekt zu speichern. 
	}
}

void PutPixel(BmpMetaData& img, int x, int y, unsigned char value)
{
	if (x >= 0 && x < (img.Width) && y >= 0 && y < (img.Height))
	{
		int index = img.PosPixelData + x + y * img.Width;
		img.Data[index] = value;
	}

	// 1. In der IF Abfrage wird abgefragt, ob die Koordinaten über Null liegen und im Pixelbereich des Bildes enthalten sind
	// 2. Ist das der Fall wird der Index im Datenarray berechnet. Die berechnung ist wie in der ELSE von GetPixel
	// 3. Der Wert des Pixels wird auf den übergebenen Wert VALUE gesetzt. (Kann Farbwert oder Index vom Pixel sein)

	//Zusammengefasst wird in dieser Funktion der Farbwert an den gegeben Koordinaten gesetzt
}
//---------------------------------------------------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------------------------------------------------
//															Sobel Berechnungen
//---------------------------------------------------------------------------------------------------------------------------------------------
float SobelX[3][3] = { { 1, 0, -1 }, { 2, 0, -2 }, {1, 0, -1} };
float SobelY[3][3] = { { -1, -2, -1, }, { 0, 0, 0, }, { 1, 2, 1 } };
//Definition der Sobel-Operatoren für die horizontale (x) und vertiakel (y)

void AppyOperator3x3(
	BmpMetaData& src, BmpMetaData& dest,
	float opx[3][3], float opy[3][3], float threshold)
{
	for (int y = 0; y < src.Height; y++)
	{
		for (int x = 0; x < src.Width; x++)
		{

			//die beiden oberen FOR-Schleifen durschlaufen jeden Pixel des Bildes. Es wird jede Reihe nacheinander abgefragt.
			int GX = 0;
			int GY = 0;



			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					GX += opx[i][j] * GetPixel(src, x - 1 + i, y - 1 + j);
					GY += opx[i][j] * GetPixel(src, x - 1 + i, y - 1 + j);
				}
			}

			// x+=a*b		-->		x= x + a*b
			//Pro Pixel werden die Sobel-Schleifen angewand. Das heißt für jeden Pixel wird eine 3x3 Matrix angelegt. Jeder Pixel wird mit seiner
			// entsprechenden Stelle aus der Matrix multipliziert.
			// Die Produkte der X-Operatoren werden zu GX zusammen addiert
			// Die Produkte der Y-Operatoren werden zu GY zusammen addiert

			float G = sqrt(GX * GX + GY * GY);

			//Anschleißend werden beide Summen unter einer ´Wurzel quadriert und addiert. Das Ergebnis is G.

			if (G > threshold)
			{
				PutPixel(dest, x, y, G);
			}
			else
			{
				PutPixel(dest, x, y, 0);
			}

			//Threshold ist der Schwellenwert, der hilft einen Vergleich zu machen.
			// Ist G größer als der Threshold, so wird er in den Pixel des Zieles geschrieben
			// Ansonsten wird der Pixel gleich 0 (schwarz) gesetzt
		}
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------------

float ThickX[3][3] = { { 0.11, 0.11, 0.11, }, { 0.11, 0.11, 0.11, }, { 0.11, 0.11, 0.11 } };
float ThickY[3][3] = { { 0.11, 0.11, 0.11, }, { 0.11, 0.11, 0.11, }, { 0.11, 0.11, 0.11 } };

int main()
{
	// Bildstruktur
	BmpMetaData sourceImg = { 0 };

	// Datei lesen
	if (!ReadFile("Lenna_pretty.bmp", sourceImg))
	{
		cout << "Fehler beim Lesen der Eingangsdatei" << endl;
		return -1;
	}

	// Header-Daten dekodieren
	if (!DecodeHeader(sourceImg))
	{
		cout << "Fehler beim Dekodieren der Header-Daten" << endl;
		return -1;
	}

	// Prozessieren
	// DrawLine(img);
	BmpMetaData sobelImg = DuplicateImage(sourceImg);
	AppyOperator3x3(sourceImg, sobelImg, SobelX, SobelY, 70.0);

	// Datei schreiben
	if (!WriteFile("Sobel_Output.bmp", sobelImg))
	{
		cout << "Fehler beim Schreiben der Datei Sobel_Output.bmp" << endl;
		return -1;
	}

	// Prozessieren
	BmpMetaData thickImg = DuplicateImage(sobelImg);
	AppyOperator3x3(sobelImg, thickImg, ThickX, ThickY, 90.0);

	// Datei schreiben
	if (!WriteFile("Thick_Output.bmp", thickImg))
	{
		cout << "Fehler beim Schreiben der Datei Thick_Output.bmp" << endl;
		return -1;
	}
}