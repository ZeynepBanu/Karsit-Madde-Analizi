#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 100
#define MAX_NAME_LENGTH 50
#define MAX_USERNAME_LENGTH 20
#define MAX_PASSWORD_LENGTH 20
#define MAX_ROWS 50
#define MAX_COLS 50
#define MAX_PARTICLES 50

// Bu versiyonda kullanýcý
//hesap oluþturabiliyor,
//giriþ yapabiliyor,
//kayýtlý olan veya kendi labirentini yükleyebiliyor,
//oyun tahtasýnda hareket ederek parçacýk toplayabiliyor,
//oyun bittiðinde oluþturulan parçacýk sayýsýna göre puan elde edebilir,
// ve skoru yeterince yüksekse skor tablosuna kaydedilir..

typedef struct {
    char name[MAX_NAME_LENGTH];
    char surname[MAX_NAME_LENGTH];
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    int score;
    int isLoggedIn;
} User;

//User dizisi, particles dizisi tanýmlanýr.
User users[MAX_USERS];
int userCount = 0;
int currentUserIndex = -1;  // -1: No user logged in
char particlesCollected[MAX_PARTICLES];
int particlesCollectedIndex=0;

typedef struct {
	char username[MAX_NAME_LENGTH];
	int score;
} ScoreBoard;

ScoreBoard  scores[5];

//Oluþturulan hesap bilgileri bin dosyasýna yazýlýr.
void saveUserDataToFile() {
    FILE *file = fopen("userdata.bin", "wb");
    if (file == NULL) {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);
    }

    fwrite(users, sizeof(User), userCount, file);

    fclose(file);
}

//Dosyadaki kullanýcý bilgileri okunur.
void loadUserDataFromFile() {
    FILE *file = fopen("userdata.bin", "rb");
    if (file == NULL) {
        return;
    }

    userCount = fread(users, sizeof(User), MAX_USERS, file);

    fclose(file);
}

//Hesap oluþturma fonksiyonu istenilendiði gibi var olan kullanýcý adlarýnýn tekrar alýnmasýna izin vermez.
void createUser() {
    if (userCount >= MAX_USERS) {
        printf("Maksimum kullanici sayisina ulasildi.\n");
        return;
    }

    User newUser;

    printf("Ad: ");
    scanf("%s", newUser.name);
    printf("Soyad: ");
    scanf("%s", newUser.surname);
    printf("Kullanici Adi: ");
    scanf("%s", newUser.username);

    // Kullanýcý adýnýn daha önce kullanýlýp kullanýlmadýðýný kontrol et
    int i;
    for (i = 0; i < userCount; i++) {
        if (strcmp(newUser.username, users[i].username) == 0) {
            printf("Bu kullanici adi zaten alinmis. Lutfen baska bir kullanici adi secin.\n");
            return;
        }
    }

    printf("Sifre: ");
    scanf("%s", newUser.password);

    newUser.score = 0;
    newUser.isLoggedIn = 0;

    users[userCount] = newUser;
    userCount++;

    printf("Hesap olusturuldu.\n");
}

//Kullanýcý login iþlemini yaparken kullanýcý adý ve þifresi kontrol edilir.
void loginUser() {
    char inputUsername[MAX_USERNAME_LENGTH];
    char inputPassword[MAX_PASSWORD_LENGTH];

    printf("Kullanici Adi: ");
    scanf("%s", inputUsername);
    printf("Sifre: ");
    scanf("%s", inputPassword);
    
    int i;
    for (i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, inputUsername) == 0 && strcmp(users[i].password, inputPassword) == 0) {
            users[i].isLoggedIn = 1;
            currentUserIndex = i;
            printf("Giris yapildi. Hosgeldiniz, %s!\n", users[i].username);
            return;
        }
    }

    printf("Giris basarisiz. Kullanici adi veya sifre hatali.\n");
}

int main() {
    loadUserDataFromFile();

    int choice;
    
    //Ýlk olarak hesap oluþturma, login iþlemleri ve oyun kurallarý seçenekleri verilir.
    while (1) {
        printf("\nKarsit Madde Labirentine Hosgeldiniz!!!\n1. Hesap Olustur\n2. Giris Yap\n3. Yuksek Skorlar\n4. Oyun Nasil Oynanir\n5. Cikis\nSeciminiz: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                createUser();
                saveUserDataToFile();
                break;
            case 2:
                loginUser();
                if (currentUserIndex != -1) {
                    mainMenu();
                }
                break;
            case 3:
            	printHighScores();
            	break;
            case 4:           	
            	printf("\nOyun Kurallari: Partikulleri toplayarak cikisa ulasin!\n");
			    break;    
            case 5:
                exit(0);
            default:
                printf("Gecersiz secim. Tekrar deneyin.\n");
        }
    }

    return 0;
}

//Kullanýcý login yaptýktan sonra olan labirentlerden seçme, kendisi yükleme, skor tablosu ve çýkýþ seçenekleri verilir.
void mainMenu() {
    int choice;

    while (1) {
        printf("\n1. Labirent Sec\n2. Kendi Labirentini Yukle\n3. Skor Tablosu\n4. Cikis\nSeciminiz: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                selectMaze();
                break;
            case 2:
                loadCustomMaze();
                break;
            case 3:
                printHighScores();
            case 4:
            	saveHighScores();
                currentUserIndex = -1;
                printf("Cikis yapildi. Ana menuye donuluyor...\n");
                return;
            default:
                printf("Gecersiz secim. Tekrar deneyin.\n");
        }
    }
}

//Yüklü olan labirentlerden birini seçer ve yükler.
void selectMaze() {
    int mazeChoice;
    char maze[MAX_ROWS][MAX_COLS];

    printf("Labirent Sec:\n1. Lab1\n2. Lab2\n3. Lab3\nSeciminiz: ");
    scanf("%d", &mazeChoice);

    char mazeFileName[10];
    switch (mazeChoice) {
        case 1:
            strcpy(mazeFileName, "Lab1.txt");
            break;
        case 2:
            strcpy(mazeFileName, "Lab2.txt");
            break;
        case 3:
            strcpy(mazeFileName, "Lab3.txt");
            break;
        default:
            printf("Gecersiz secim. Ana menuye donuluyor...\n");
            return;
    }

    FILE *file = fopen(mazeFileName, "r");
    if (file == NULL) {
        perror("Error opening maze file");
        exit(EXIT_FAILURE);
    }

    int rows, cols;
    int i;
    fscanf(file, "%d %d", &rows, &cols);

    for (i = 0; i < rows; i++) {
        fscanf(file, "%s", maze[i]);
    }

    fclose(file);

    printf("Labirent secildi. Oyun basliyor...\n");
    playGame(rows, cols, maze);
}

//Kullanýcýdan belirli bir þekilde labirent ismi ister ve labirenti yükler.
void loadCustomMaze() {
    char mazeFileName[50];
    char maze[MAX_ROWS][MAX_COLS];

    printf("Kendi labirent dosyanizi girin (Lab4.txt gibi): ");
    scanf("%s", mazeFileName);
    
    
    // Oyunu baþlatmak için özel labirenti yükle
    FILE *file = fopen(mazeFileName, "r");
    if (file == NULL) {
        perror("Error opening maze file");
        exit(EXIT_FAILURE);
    }

    int rows, cols;
    int i;
    fscanf(file, "%d %d", &rows, &cols);

    for (i = 0; i < rows; i++) {
        fscanf(file, "%s", maze[i]);
    }

    fclose(file);

    printf("Labirent yuklendi. Oyun basliyor...\n");
    playGame(rows, cols, maze);

}

//Labirentte kullanýcý inputlarýyla oyunu oynatan fonksiyon.
void playGame(int rows, int cols, char maze[MAX_ROWS][MAX_COLS]) {
    int userRow, userCol;
    int i,j;

    // Kullanýcýnýn giriþ yeri bulunur.
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (maze[i][j] == 'G') {
                userRow = i;
                userCol = j;
            }
        }
    }

    char mazeTemp[MAX_ROWS][MAX_COLS];
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            mazeTemp[i][j] = maze[i][j];
        }
    }
    
    printMaze(rows, cols, maze);


    while (1) {
        
        char move;

        //Hareket etme tuþlarý ve çýkýþ tuþu kullanýcýya verilir.
        printf("Hareket etmek icin ok tuslarini kullanin. Cikis icin Q veya ESC tusuna basiniz.\n\n\n\n");
        
        move = getch();
        
        
        // yukarý 72, aþagý 80, sag 77, sol 75
        // Kullanýcý inputuna göre yeni konumlar belirlenir.
        if (move == 72) {
            int newRow = userRow - 1;
            if (newRow >= 0 && maze[newRow][userCol] != '1') {
                movePlayer(maze, mazeTemp, &userRow, &userCol, newRow, userCol);
            }
        } else if (move == 75) {
            int newCol = userCol - 1;
            if (newCol >= 0 && maze[userRow][newCol] != '1') {
                movePlayer(maze, mazeTemp, &userRow, &userCol, userRow, newCol);
            }
        } else if (move == 80) {
            int newRow = userRow + 1;
            if (newRow < rows && maze[newRow][userCol] != '1') {
                movePlayer(maze, mazeTemp, &userRow, &userCol, newRow, userCol);
            }
        } else if (move == 77) {
            int newCol = userCol + 1;
            if (newCol < cols && maze[userRow][newCol] != '1') {
                movePlayer(maze, mazeTemp, &userRow, &userCol, userRow, newCol);
            }
        } else if (move == 'Q' || move == 'q' || move == 27) {
            printf("Oyun Cikis Yapti.\n");
            return;
        } else {
            printf("Gecersiz hareket. Tekrar deneyin.\n");
        } 
        
        printMaze(rows, cols, maze);
        printParticlesCollected();
        
    }
}

int getch() {
#ifdef _WIN32
    return _getch();
#else
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
#endif
}

void printMaze(int rows, int cols, char mazeTemp[MAX_ROWS][MAX_COLS]) {
	int i,j;
    printf("Labirent:\n");
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%c ", mazeTemp[i][j]);
        }
        printf("\n");
    }
    

}

//Yeni konumlar kullanýlarak kullanýcý konumu güncellenir.
void movePlayer(char maze[MAX_ROWS][MAX_COLS], char mazeTemp[MAX_ROWS][MAX_COLS], int *userRow, int *userCol, int newRow, int newCol) {
	if (mazeTemp[*userRow][*userCol]== 'p' || mazeTemp[*userRow][*userCol]== 'P' || mazeTemp[*userRow][*userCol]== 'e' || mazeTemp[*userRow][*userCol]== 'E'){
		addParticle(mazeTemp[*userRow][*userCol]);
		maze[*userRow][*userCol]= '0';
	} 
	else if(mazeTemp[*userRow][*userCol]== 'K'){
		
		printf("Kaybettiniz!!! Devam etmek icin bir tusa basiniz.");
		scanf("%c");
		exit(0);
	}
	else if(mazeTemp[*userRow][*userCol]== 'C'){
		
		printf("Oyun bitti!!! Devam etmek icin bir tusa basiniz.");
		scanf("%c");
		
		int numKMatter=calculateKMatter();
		
		getScoreandList(numKMatter);
		
		exit(0);
	}
	else {
		maze[*userRow][*userCol] = mazeTemp[*userRow][*userCol];  // Eski konumda olan deðeri yerleþtir.
	}
    *userRow = newRow;
    *userCol = newCol;
    mazeTemp[*userRow][*userCol] = maze[*userRow][*userCol];  // Yeni konumu yerleþtir.
    maze[*userRow][*userCol] = 'X';  // Yeni konuma yerleþtir.
}

//Karþýt Madde oluþmasýný hesaplar.
int calculateKMatter()
{
			int i=0;
			int numP = 0;
			int nump = 0;
			int numE = 0;
			int nume = 0;
			int numKMatter=0;
			
			int netp;
			int netE;
			
			for(i=0; i< particlesCollectedIndex; i++){
				
			    if(particlesCollected[i]== 'P'){
			    	numP++;
				} else if(particlesCollected[i]== 'p'){
			    	nump++;
				} else if(particlesCollected[i]== 'E'){
			    	numE++;
				} else if(particlesCollected[i]== 'e'){
			    	nume++;
				}
			}
			
			netp = nump - numP;
		
			netE = numE - nume;
			
			if(netE>=0 && netp>=0){
				
				if(netE>=netp){
					numKMatter=netp;
				} else {
					
					numKMatter=netE;
				}
			}
		
			printf("\n %d adet karsit madde uretildi!!\n", numKMatter);
			return numKMatter;
}

//Skor tablosuna yebi geðer ekler.
void getScoreandList(int numKMatter){
	
	int score = numKMatter*100;
	loadHighScores();
	
	if(scores[4].score < score){
		scores[4].score = score;
		strcpy(scores[4].username,scores[4].username);
    	saveHighScores();	
	}
	
}

//Skor tablosunu günceller.
void sortScoreBoard(){
	
	ScoreBoard temp;
	int i,j;
	
	for(i=0; i<4; i++){
		for(j=i+1; j<5;j++){
			if(scores[j].score>scores[i].score){
				temp.score=scores[i].score;
				strcpy(temp.username,scores[i].username);
				scores[i].score=scores[j].score;
				strcpy(scores[i].username,scores[j].username);
				scores[j].score=temp.score;
				strcpy(scores[j].username,temp.username);
			}
		}
	}
	
}

void printHighScores(){
	
    loadHighScores();
    
    int i;
    
    for(i=0; i<5; i++){
        printf("%s : %d\n", scores[i].username, scores[i].score);
    }
    printf("\n");
}

//Skorlarý text dosyasýna saklar.
void saveHighScores() {
    FILE *file = fopen("HighScore.txt", "w");
    if (file == NULL) {
        printf("Error opening HighScore file for writing.\n");
    }

	sortScoreBoard();

    int i;
    for (i = 0; i < 5; i++) {
        fprintf(file, "%s %d\n", scores[i].username, scores[i].score);
    }

    fclose(file);
}

// Skorlarý text dosyasýndan okur.
void loadHighScores() {
    FILE *file = fopen("HighScore.txt", "r");
    if (file == NULL) {
        printf("Error opening HighScore file for reading.\n");
    }
    int i;
    for (i = 0; i < 5; i++) {
        fscanf(file, "%s %d", scores[i].username, scores[i].score);
    }

    fclose(file);
}

//Toplanan partikülleri diziye kaydeder.
void addParticle(char particle)
{
	particlesCollected[particlesCollectedIndex] = particle;
	particlesCollectedIndex++;
}

//Toplanan partikülleri yazdýrýr.
void printParticlesCollected(){
	int i;
	
	printf("\nToplanan Atom Alti Parcacikler: ");
	
	for(i = 0; i < particlesCollectedIndex; i++){
		
		printf("%c ", particlesCollected[i]);
		
	}
	printf("\n");
}
