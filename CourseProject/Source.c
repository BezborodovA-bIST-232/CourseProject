#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#define RUN 32

struct software
{
	char Name[100];
	char Type[100];
	float Version;
	char Developer[100];
	char License[100];
};

typedef struct software Software;

struct array {
	Software* array;
	int size;
};

typedef struct array Array;

void PrintSoftware(Array arr, int index);
Software SoftwareInit();
Array AddElement(Array arr, Software software);
int FindByName(Array arr, char* str);
Array FindByLicense(Array data, Array list, char* str);
void FindInDatabase(Array database, char second_choose, char* str);
Array DelElement(Array arr, int index);
int SaveToFile(Array arr);
Array LoadFromFile();
void SortInsert(Software* ptrarr, int left, int right);
void Merge(Software* ptrarr, int l, int m, int r);
void TimSort(Software* ptrarr, int n);

int main() {
	setlocale(LC_ALL, "RUS");
	system("chcp 1251");

	Array database;
	database.size = 0;
	database.array = (Software*)malloc(database.size * sizeof(Software));
	int run = 1;

	while (run) {
		//Основное меню
		system("cls");
		printf("1. Вывести базу данных\n");
		printf("2. Добавить запись в базу данных\n");
		printf("3. Удалить запись из базы данных(по имени)\n");
		printf("4. Поиск по параметру в базе данных\n");
		printf("5. Записать в файл\n");
		printf("6. Загрузить базу данных из файла\n");
		printf("7. Отсортировать базу данных по алфавиту в поле \"Разработчик\"\n");
		printf("8. Выйти из программы\n");
		printf("Выберите пункт: ");

		char choose, second_choose;
		scanf(" %c", &choose);

		switch (choose)
		{
			//1. Вывести базу данных
		case '1':
			system("cls");
			printf("База данных\n");
			printf("%5c| %20.20s | %20.20s | %8.8s | %20.20s | %20.20s |\n", ' ', "Название", "Тип ПО", "Версия", "Разработчик", "Лицензия");
			for (int i = 0; i < database.size; i++) PrintSoftware(database, i);
			system("pause");
			break;
			//2. Добавить запись в базу данных
		case '2':
			system("cls");
			Software software = SoftwareInit();
			database = AddElement(database, software);
			break;
			//3. Удалить запись из базы данных(по имени)
		case '3':
			system("cls");
			printf("Введите название ПО: ");
			char str[200];

			scanf(" %[^\n]s", &str);
			str[strlen(str)] = '\0';

			int index = FindByName(database, &str);
			if (index == -1) {
				printf("Извините, в данный момент в базе данных нет данных с такими значениями.\n");
			}
			else {
				printf("%5c| %20.20s | %20.20s | %8.8s | %20.20s | %20.20s |\n", ' ', "Название", "Тип ПО", "Версия", "Разработчик", "Лицензия");
				PrintSoftware(database, index);
			}

			printf("Вы хотите удалить эту запись? (y / n): ");
			scanf(" %c", &second_choose);

			if (second_choose == 'y') {
				database = DelElement(database, index);
			}
			system("pause");
			break;
			//4. Поиск по параметру в базе данных
		case '4':
			system("cls");

			printf("Поиск по параметру:\n");
			printf("%5c1. Название\n", ' ');
			printf("%5c2. Вид лицензии\n", ' ');
			printf("Выберите пункт: ");

			scanf(" %c", &second_choose);

			printf("Введите нужное вам значение поля: ");
			scanf(" %[^\n]s", &str);
			str[strlen(str)] = '\0';

			FindInDatabase(database, second_choose, str);
			system("pause");
			break;
			//5. Записать в файл
		case '5':
			system("cls");
			printf("Вы уверены, что хотите перезаписать файл? (y / n): ");

			scanf(" %c", &second_choose);
			if (second_choose == 'y') {
				int result = SaveToFile(database);
				if (result == 0) printf("Базу данных не получилось записать в файл\n");
				if (result == 1) printf("База данных успешно записана в файл\n");
				system("pause");
			}
			break;
			//6. Загрузить базу данных из файла
		case '6':
			system("cls");
			printf("Вы уверены, что хотите загрузить базу данных из файла? (y / n): ");

			scanf(" %c", &second_choose);
			if (second_choose == 'y') {
				free(database.array);
				database.size = 0;
				database.array = (Software*)malloc(database.size * sizeof(Software));
				Array temp = LoadFromFile();
				database.array = temp.array;
				database.size = temp.size;
				system("pause");
			}
			break;
			//7. Отсортировать базу данных по алфавиту в поле "Разработчик"
		case '7':
			system("cls");
			TimSort(database.array, database.size);
			printf("база данных успешно отсортированна\n");
			system("pause");
			break;
			//8. Выйти из программы
		case '8':
			run = 0;
			break;
		}
	}
	free(database.array);
	return 0;
}

void PrintSoftware(Array arr, int index) {

	printf("%5c| %20.20s | %20.20s | %8.2f | %20.20s | %20.20s |\n", ' ', arr.array[index].Name, arr.array[index].Type, arr.array[index].Version, arr.array[index].Developer, arr.array[index].License);
}

Software SoftwareInit() {
	Software software = { "", "", 1., "", "" };
	printf("Добавить в базу данных программное обеспечение\n");

	printf("%5cНазвание: ", ' ');
	scanf(" %[^\n]s", software.Name);

	printf("%5cТип ПО: ", ' ');
	scanf(" %[^\n]s", software.Type);
	printf("%5cВерсия: ", ' ');
	scanf("%f", &software.Version);

	printf("%5cРазработчик: ", ' ');
	scanf(" %[^\n]s", software.Developer);

	printf("%5cВид Лицензии: ", ' ');
	scanf(" %[^\n]s", software.License);
	return software;
}

Array AddElement(Array arr, Software software) {
	int size_n = arr.size + 1;
	Software* temp = (Software*)realloc(arr.array, size_n * sizeof(Software));
	arr.array = temp;
	arr.array[size_n - 1] = software;
	arr.size = size_n;
	return arr;
}

int FindByName(Array arr, char* str) {
	for (int i = 0; i < arr.size; i++) {
		if (strcmp(arr.array[i].Name, str) == 0) {
			return i;
		}
	}
	return -1;
}

Array FindByLicense(Array data, Array list, char* str) {
	for (int i = 0; i < data.size; i++) {
		if (strcmp(data.array[i].License, str) == 0) {
			int size_l_n = (list.size) + 1;
			Software* temp = (Software*)realloc(list.array, size_l_n * sizeof(Software));
			list.array = temp;
			list.array[size_l_n - 1] = data.array[i];
			list.size = size_l_n;
		}
	}
	return list;
}

void FindInDatabase(Array database, char second_choose, char* str) {
	switch (second_choose)
	{
	case '1':;
		int index = FindByName(database, str);
		if (index == -1) {
			printf("Извините, в данный момент в базе данных нет данных с такими значениями.\n");
			break;
		}
		printf("%5c| %20.20s | %20.20s | %8.8s | %20.20s | %20.20s |\n", ' ', "Название", "Тип ПО", "Версия", "Разработчик", "Лицензия");
		PrintSoftware(database, index);
		break;
	case '2':;
		Array list;
		list.size = 0;
		list.array = (Software*)malloc(list.size * sizeof(Software));
		list = FindByLicense(database, list, str);
		if (list.size == 0) {
			printf("Извините, в данный момент в базе данных нет данных с такими значениями.\n");
			break;
		}
		printf("%5c| %20.20s | %20.20s | %8.8s | %20.20s | %20.20s |\n", ' ', "Название", "Тип ПО", "Версия", "Разработчик", "Лицензия");
		for (int i = 0; i < list.size; i++) PrintSoftware(list, i);
		free(list.array);
		break;
	default:
		break;
	}
}

Array DelElement(Array arr, int index) {
	Software empty = { "", "", 1., "", "" };
	if (index == arr.size - 1) {
		arr.array[index] = empty;
	}
	else {
		Software copy;
		copy = arr.array[arr.size - 1];
		arr.array[index] = copy;
	}

	int size_n = arr.size - 1;
	Software* temp = (Software*)realloc(arr.array, size_n * sizeof(Software));
	arr.array = temp;
	arr.size = size_n;
	return arr;
}

int SaveToFile(Array arr) {
	FILE* file = fopen("data.txt", "w");
	if (file == NULL) return 0;
	for (int i = 0; i < arr.size; i++) {
		if (file)
		{
			if (i != 0) fprintf(file, "\n");
			fprintf(file, "%s\n", arr.array[i].Name);
			fprintf(file, "%s\n", arr.array[i].Type);
			fprintf(file, "%f\n", arr.array[i].Version);
			fprintf(file, "%s\n", arr.array[i].Developer);
			fprintf(file, "%s\0", arr.array[i].License);
		}
	}
	fclose(file);
	return 1;
}

Software SoftwareFileInit(FILE* file) {

	Software software;
	fgets(software.Name, 100, file);
	fgets(software.Type, 100, file);
	fscanf(file, "%f\n", &software.Version);
	fgets(software.Developer, 100, file);
	fgets(software.License, 100, file);

	strtok(software.Name, "\n");
	strtok(software.Type, "\n");
	strtok(software.Developer, "\n");
	strtok(software.License, "\n");

	return software;
}

Array LoadFromFile() {
	Array temp;
	temp.size = 0;
	temp.array = (Software*)malloc(temp.size * sizeof(Software));

	FILE* file = fopen("data.txt", "r");
	if (file == NULL) {
		printf("Ошибка чтения файла\n");
	}
	while (feof(file) == 0) {
		Software software = SoftwareFileInit(file);
		temp = AddElement(temp, software);
	}
	fclose(file);
	printf("База данных успешно загружена из файла\n");
	return temp;
}

void SortInsert(Software* ptrarr, int left, int right) {
	for (int i = left + 1; i <= right; i++) {
		Software temp = ptrarr[i];
		int j = i - 1;
		while (j >= left && strcmp(ptrarr[j].Developer, temp.Developer) > 0) {
			ptrarr[j + 1] = ptrarr[j];
			j--;
		}
		ptrarr[j + 1] = temp;
	}
}

void Merge(Software* ptrarr, int l, int m, int r)
{
	int len1 = m - l + 1, len2 = r - m;
	Software* left = (Software*)malloc(len1 * sizeof(Software));
	Software* right = (Software*)malloc(len2 * sizeof(Software));
	for (int i = 0; i < len1; i++)
		left[i] = ptrarr[l + i];
	for (int i = 0; i < len2; i++)
		right[i] = ptrarr[m + 1 + i];

	int i = 0;
	int j = 0;
	int k = l;

	while (i < len1 && j < len2) {
		if (strcmp(left[i].Developer, right[j].Developer) > 0) {
			ptrarr[k] = left[i];
			i++;
		}
		else {
			ptrarr[k] = right[j];
			j++;
		}
		k++;
	}
	while (i < len1) {
		ptrarr[k] = left[i];
		k++;
		i++;
	}
	while (j < len2) {
		ptrarr[k] = right[j];
		k++;
		j++;
	}
}

void TimSort(Software* ptrarr, int n)
{
	for (int i = 0; i < n; i += RUN)
		SortInsert(ptrarr, i, min((i + RUN - 1), (n - 1)));
	for (int size = RUN; size < n; size = 2 * size) {
		for (int left = 0; left < n; left += 2 * size) {
			int mid = left + size - 1;
			int right = min((left + 2 * size - 1), (n - 1));
			if (mid < right)
				Merge(ptrarr, left, mid, right);
		}
	}
}