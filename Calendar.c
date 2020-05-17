#include <stdio.h>
#include <stdlib.h>

// function declaration
int isLeapYear(int year);
int leapYears(int year);
int todayOf(int y, int m, int d); 
long days(int y, int m, int d);
void calendar(int y, int m);
int getDayNumber(int d,int m,int y);
char *getName(int day);
 

void flush()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
 
typedef struct {
  int day;
  int month;
  int year;
  char note[255];
} Note;

// main function
int main(int argc, char* argv[]) {
  
  int year, month, day;
  char choice;
  Note note;
  FILE *fp;
 
  fp = fopen("note.bin", "r");
  if (fp == NULL) {
    fp = fopen("note.bin", "w");
  } 
  fclose(fp);
  
  // menu asking the user how to proceed
  while(1) {
    printf("\n–––––––––––––––––––––––––––––\n");
    printf("1. Find the day of a date\n");
    printf("2. Print calendar of a month\n");
    printf("3. Add Note to a date\n");
    printf("4. Exit\n");
    printf("–––––––––––––––––––––––––––––\n");
    printf("\nEnter your choice: ");
    scanf("\n%c", &choice);

    // go to specific case and execute it based on the users choice
    switch(choice) {
      // if user inputs '1': print weekday of specific date
      case '1':
      printf("\nEnter the day, month and year (DD/MM/YYY): ");
      scanf("%d/%d/%d", &day, &month, &year);
      printf("This day [%d/%d/%d] is a %s.\n", day, month, year, getName(getDayNumber(day, month, year)));
      break;

      // if user inputs '2': print calender of a given month an print notes of days in this month if user inputs 's'
      case '2':
      printf("\nEnter the month and year (MM/YYYY): ");
      scanf("%d/%d", &month, &year);
      printf("\nPress 's' to see the notes or any other key to continue.\n");
      calendar(year, month);
      break;

      // if user inputs '3': add note to a given day chosen by user based on his input
      case '3':
      printf("Enter the day, month and year (DD/MM/YYY): ");
      scanf("%d/%d/%d", &note.day, &note.month, &note.year);
      flush();
      printf("Enter the note: ");
      fgets(note.note, 255, stdin);

      // open file in 'reading & appending mode' and put input note in file and print confirmation
      fp = fopen("note.bin", "a+");
      if (fp == NULL) {
        printf("File note.bin can not be opened\n");
        exit(1);
      }
      fwrite(&note, sizeof(Note), 1, fp);
      printf("\nNote added sucessfully\n\n");
      fclose(fp);
      break;

      // if user inputs '4': stop program and return to menu
      case '4':
      printf("See you next time!");
      exit(0);
      break;

      // return function if user input doesn' correspont to one of the options 1-4
      default:
      printf("Not a valid option\n");
      break;
    }
  }
  return 0;
}

// function checking, if corresponding year is a leap year (returns true, if it's a leap year)
int isLeapYear(int y){
  return(y % 400 == 0) || ((y % 4 == 0) && (y % 100 != 0));
}

// function returning the number of leap years
int leapYears(int y){
  return y/4 - y/100 + y/400;
}

// function returning the number of days since the beginning of the year
int todayOf(int y, int m, int d) {
  static int DayOfMonth[] = 
    { -1, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };
  return DayOfMonth[m] + d + ((m > 2 && isLeapYear(y))? 1 : 0);
}

// function returning the total number of days
long days(int y, int m, int d) {
  int lastYear;
  lastYear = y - 1;
  return 365L * lastYear + leapYears(lastYear) + todayOf(y, m, d);
}

// function displaying the calender with month and year
void calendar(int y, int m) {
  FILE *fp;
  Note* notes, note;
  int len, j, hasNote = 0;
  char choice;
  const char *NameOfMonth[] = { NULL,
    "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"
  };
  char Week[] = " Su    Mo    Tu    We    Th    Fr    Sa";
  char line[] = "–––––––––––––––––––––––––––––––––––––––";
  int DayOfMonth[] =
    { -1, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
  int weekOfTopDay;
  int i, day;
 
  weekOfTopDay = days(y, m, 1) % 7;
  
  // open file in reading mode (rb -> binary) and tell the user, if there are no notes stored
  fp = fopen("note.bin", "rb");
  if (fp == NULL) {
    printf("Couldn't read notes\n");
  }
  // check for all stored notes
  len = 0;
  while(fread(&note, sizeof(Note), 1, fp)) {
    if (note.year == y && note.month == m) {
      len++;
    }
  }
  rewind(fp);
  j = 0;
  notes = (Note*) malloc (sizeof(Note) * len);
  while(fread(&note, sizeof(Note), 1, fp)) {
    if (note.year == y && note.month == m) {
      notes[j] = note;
      j++;
    }
  }
  fclose(fp);
 
 // change number of days of February to 29 if leap year and print header of table (month&year and days)
  if(isLeapYear(y))
    DayOfMonth[2] = 29;
  printf("\n            %s %d\n%s\n%s\n", NameOfMonth[m], y, line, Week);
 
  // leave fields free until the first day of the month appears
  for(i = 0; i < weekOfTopDay; i++)
    printf("      ");

  // print dates correctly in calendar screen
  for(i = weekOfTopDay, day = 1; day <= DayOfMonth[m]; i++, day++) {
    hasNote = 0;

    // if any day of month has a note, highlight it (using '| |') for every corresponding day
    for (j = 0; j < len; j++) {
      if (notes[j].day == day) {
        printf("|%2d|  ", day);
        hasNote = 1;
        break;
      }
    }
    // if every day of month has no note, just print the day without highlighten it
    if (hasNote == 0) {
      printf(" %2d   ", day);
    }
    // go to next line, if line is full
    if(i % 7 == 6)
      printf("\n");
  }

  // ask user if he wants to read the notes --> list them if answer is 's'
  printf("\n");
  scanf("\n%c", &choice);
  if (choice == 's') {
    printf("Here is the list of notes for %d/%d:\n", m, y);
    for (j = 0; j < len; j++) {
      printf("%d/%d/%d: %s", notes[j].day, notes[j].month, notes[j].year, notes[j].note);
    }
  }
  else {
    return;
  }
}

// function returning the number of the day
int getDayNumber(int d, int m, int y) {
  static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
  y -= m < 3;
  return (y + y/4 - y/100 + y/400 + t[m-1] + d) % 7;
}

// function returning the name of the day 
char *getName(int day) {
  switch(day){
    case 0: return("Sunday");
    case 1: return("Monday");
    case 2: return("Tuesday");
    case 3: return("Wednesday");
    case 4: return("Thursday");
    case 5: return("Friday");
    case 6: return("Saturday");
    default:return("Error: invalid argument");
  }
}
