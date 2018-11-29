#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>

#define max 20
#define width 100
#define path_size 50
#define m_words 100
#define Queue_Size 5

void clear_newlines(void)
{
	int c;
	do
	{
		c=getchar();
	}while(c!='\n' && c!=EOF);
}

typedef struct node
{
	char word[width];
	struct node* next;
}node;

FILE* Create();
void Delete(char path[path_size]);
void Edit(char path[path_size]);
void Append(char path[path_size], char data[width]);
void Display(FILE *fin);
void menu(char k[path_size]);
void Undo(char path[path_size],int choice, char data[width], char ret[width]);
void PUSH(char stack[max][width], int *top, char data[width]);
void POP(char stack[max][width], int *top, char m[width]);
void ENQUEUE(node **first, char data[width]);
void DEQUEUE(node **first);
void DEL_QUEUE(node **first);
void Rename(char path[path_size], char new_path[path_size]);
int Find(char path[path_size], char data[width]);
void Replace(char path[path_size],char word[width],char replace[width]);
int Spell_Check(char path[path_size], char wrong[m_words][width]);
int Dictionary(char word[width]);


void main()
{
  FILE *fin, *fout;

  char z;
  char k[path_size];
  printf("Do You Want to Open an Already Existing File?(Y?N): ");
  scanf("%c", &z);
  clear_newlines();
  if(toupper(z)=='Y')
  {
    printf("Enter File Path: ");
    scanf("%s",k);
    //printf("%s\n",k );
    fin=fopen(k,"r+");
    fflush(stdin);
    if(fin==NULL)
    {
      printf("Invalid File Path\n");
      return;
    }
    else
    {
      fclose(fin);
    }
  }
  else if(toupper(z)=='N') /*If User Says No, Create a New File*/
  {
  	char new_name[width], old_name[width];
		printf("Enter The Name/Path of The File to be Created: ");
    fflush(stdin);
		scanf("%s",new_name);
    fin = Create();
    fclose(fin);
    strcpy(k, new_name);
    strcpy(old_name,"./New_File.txt");
    rename(old_name, new_name);
    strcpy(k, new_name);
  }
  menu(k);
}

void menu(char path[path_size])
{
  int x, r_top=-1;
  char *k="Junk";
  char re[width];
  char r_arr[max][width];/*Redo-Word-Stack*/
  int n;
  do
  {
    printf("\n\n******|TEXT_EDITOR MENU|****** \n");
    printf("1.Append Text to The File\n2.Undo\n3.Redo\n4.View The File\n5.Rename The File\n6.Delete The File\n7.Spell Check\n8.Find\n9.Find And Replace\n10.Exit\nEnter Choice: ");
    scanf("%d", &x);
    clear_newlines();
    if(x==1)
    {
      char t;
      char data[width]={'\0'};
      printf("How many lines do you want to append? ");
      scanf("%d",&n);
      clear_newlines();
      int i;
      printf("Enter the Text to be Appended: \n");
      for(i=0;i<n;i++)
      {
        fgets(data, width, stdin);
        fflush(stdin);
        Append(path, data);
      }
    }
    if(x==2)
    {
			/*Undoes the Most Recent Operation and Pushes the line into the Redo-Word-Stack
			Since the Stack Follows LIFO, The Most Recent Undone action is Redone.*/
      Undo(path,1,k,re);
      PUSH(r_arr, &r_top, re);
    }
    if(x==3)
    {
      if(r_top==-1) /*All Possible Actions are Redone, So the Stack is Empty*/
      {
        printf("No Action to Redo.\n");
      }
      else
      {
        char dc[width];
        Append(path,r_arr[r_top]);
        POP(r_arr, &r_top, dc);/*After Appendinding, Pop the Redo-Stack*/
      }

    }
    if(x==4)
    {
      FILE *f_disp=fopen(path,"r");
      Display(f_disp);
      fclose(f_disp);
    }
    if(x==5)
    {
      char new_path[path_size];
      printf("Enter The New File Name/Path: ");
      scanf("%s", new_path);
      Rename(path, new_path);
    }
    if(x==6)
    {
      Delete(path);
    }

		if(x==7)
		{
			char wrong[m_words][width];/*Array of All the Missplelt Words*/
			int count=Spell_Check(path, wrong);
			if(count!=0)
			{
				printf("\nNumber of Misspelt Words: %d \n", count);
				int i=0;
				for(i=0; i<count; i++)
				{
					printf("%s \n", wrong[i]);
				}
			}
			else
			{
				printf("No Misspelt Words\n");
			}
		}

    if(x==8)
    {
    	char word[width], temp[width];
    	printf("Enter The Word to be Found: ");
    	scanf("%[^\n]", word);
    	strcpy(temp, word);
    	int count=Find(path, word);
			if(count!=0)
			{
    		printf("The word '%s' was found %d times.\n", temp, count);
			}
			else
			{
				printf("Word Not Found.\n");
			}

    }

    if(x==9)
    {
    	char word[width], replace[width], temp[width];
    	printf("Enter The Word to be Replaced: ");
    	scanf("%[^\n]", word);
    	clear_newlines();
    	printf("Enter The New Word: ");
    	scanf("%[^\n]", replace);
    	strcpy(temp, word);
    	int p=Find(path, word);
			if(p!=0)
    	{
				Replace(path, temp, replace);
    		printf("The word '%s' was replaced %d times by '%s'.\n", temp, p, replace);
			}
			else
			{
				printf("Word Not Found.\n");
			}
    }

  }while(x<10);
}

/*Displays Contents of The File*/
void Display(FILE *fin)
{
	printf("\n");
	while(!feof(fin))
	{
	  char c=getc(fin);
	  printf("%c",c);
	}
}

/*Creates a New File*/
FILE* Create()
{
  FILE *fin;
  fin=fopen("./New_File.txt", "w+");
  return fin;
}

/*Deletes The File*/
void Delete(char path[path_size])
{
  if(remove(path)==0)
  {
    printf("File Deleted.\n");
  }
  else
  {
    printf("Error, Deletion Could not be Performed\n");
  }
}
/*Appends Data To the File*/
void Append(char path[path_size], char data[width])
{
  FILE *fin = fopen(path, "a");
  if(fin==NULL)
  {
  	printf("Error in Opening File \n");
  	return;
  }
  char p[width];
  Undo(path, 0, data, p);/*To Push the Appended data to the Undo-Word-Stack*/
  fprintf(fin,"%s",data);
  fclose(fin);
}

void PUSH(char stack[max][width], int *top, char data[width])
{
	if(*top == max -1)
  {
		/*If the Size of the array is exceeded, Then the Least Recent Action
		is Removed From from the Stack and the Most Recent action is Pushed into it*/
    int i=0;
    for(i=1;i<max;i++)
    {
      strcpy(stack[i-1], stack[i]);
    }
    strcpy(stack[max-1], data);
  }
  else
	{
		*top = *top + 1;
		strcpy(stack[*top], data);
	}
}

void POP(char stack[max][width], int *top, char m[width])
{
	if(*top==-1)
	{
		*top=0;
	}
	else
	{
		strcpy(m,stack[*top]);
    *top = *top-1;
	}
}

void ENQUEUE(node **first, char data[width])
{
	node *new=(node*)malloc(sizeof(node));
	strcpy(new->word, data);
	new->next=NULL;

	if(*first==NULL)
	{
		*first=new;
	}

	else
	{
		node* temp= *first;
		while(temp->next!=NULL)
		{ temp=temp->next;}
		temp->next=new;
	}

}

void DEQUEUE(node **first)
{
	if(*first==NULL)
	{
		printf("Empty LL \n");
		return;
	}

	node* temp= *first;
	*first=(*first)->next;
	temp->next=NULL;
	free(temp);
}

void DEL_QUEUE(node **first)
{
	node *temp=*first;
	node *next=temp->next;
	while(temp!=NULL)
	{
		next=temp->next;
		free(temp);
		temp=next;
	}
	 *first=NULL;
	 free(*first);
}

void Display_STRSTK(char arr[max][width], int top)
{
  if(top==-1)
  {
    printf("Empty Stack");
  }
  else
  {
    for(top;top>-1;top--)
    {
      printf("%s->",arr[top]);
    }
    printf("\n");
  }
}
/*To Undo The Most Recent Action*/
void Undo(char path[path_size], int choice, char data[width], char ret[width])
{
	/*Variables are made static because Undo is called Multiple times
	and the Stack and Top should not be Re-Initialized.*/
  static char arr[max][width];
  static int top=-1;
  static int flag=-1;
  int c=choice;
  FILE *f_edit;
  f_edit=fopen(path, "a+");
  static long unsigned int m;
  if(c==0)
  {
    flag=0;
    PUSH(arr, &top, data);
    fclose(f_edit);
    strcpy(ret, data);
  }

  else if(c==1)
  {
      if(top==0)
      {
				m=strlen(arr[top]);
        FILE* f_new= fopen("./a.txt", "w+");
        fseek(f_edit,-m,SEEK_END);
        long int n=ftell(f_edit);
        fseek(f_edit,0,SEEK_SET);

        while(ftell(f_new)!=n)
        {
          char ch=fgetc(f_edit);
          fputc(ch, f_new);
        }
        fclose(f_edit);
        remove(path);
        fclose(f_new);
        rename("./a.txt", path);
        POP(arr, &top, ret);
        printf("Nothing More to Undo.\n");
      }
      else
      {
				m=strlen(arr[top]);
        FILE* f_new= fopen("./a.txt", "w+");
        fseek(f_edit,-m,SEEK_END);
        long int n=ftell(f_edit);
        fseek(f_edit,0,SEEK_SET);

        while(ftell(f_new)!=n)
        {
          char ch=fgetc(f_edit);
          fputc(ch, f_new);
        }
        fclose(f_edit);
        remove(path);
        fclose(f_new);
        rename("./a.txt", path);
        POP(arr, &top, ret);
      }
  }

}
/*Rename A File*/
void Rename(char path[path_size], char new_path[path_size])
{
  FILE* f_rename;
  f_rename=fopen(path, "a+");
  rename(path, new_path);
  fclose(f_rename);
}
/*Finds number Occurances of the Word, NOT CASE-SENSITIVE*/
int Find(char path[path_size], char data[width])
{
	  FILE *fp1 = fopen(path, "r");
	  int count=0;
	  char string[width];
	  int k=strlen(data);
	  int i=0;
	  for(i=0;i<k;i++)
	  {
	  	data[i]=tolower(data[i]);
	  }

	  if (!fp1)
      {
         printf("Unable to open the input file!!\n");
         return 0;
      }
	  while (!feof(fp1))
        {
                strcpy(string, "\0");
                fgets(string, width, fp1);
								int l=strlen(string);
								for(i=0;i<l;i++)
								{
				  				string[i]=tolower(string[i]);
	  						}
	  						if (strstr(string, data))
                {
                	count++;
                }
         }
      fclose(fp1);
	  return count;
}
/*Finds and Replaces all Occurences of a word, CASE-SENSITIVE*/
void Replace(char path[path_size],char word[width],char replace[width])
{
        FILE *fp1, *fp2;
        char string[width];
        char temp[] = "temp.txt", *ptr1, *ptr2;
        fp1 = fopen(path, "r");
        if (!fp1)
        {
                printf("Unable to open the input file!!\n");
        }

        fp2 = fopen(temp, "w");
        if (!fp2)
        {
                printf("Unable to open temporary file!!\n");
        }

        while (!feof(fp1))
        {
                strcpy(string, "\0");
                fgets(string, width, fp1);
                if (strstr(string, word))
                {
                        ptr2 = string;
                        while (ptr1 = strstr(ptr2, word))
                        {
                                while (ptr2 != ptr1)
                                {
                                        fputc(*ptr2, fp2);
                                        ptr2++;
                                }
                                ptr1 = ptr1 + strlen(word);
                                fprintf(fp2, "%s", replace);
                                ptr2 = ptr1;
                        }
                        while (*ptr2 != '\0')
                        {
                                fputc(*ptr2, fp2);
                                ptr2++;
                        }
                }
                else
                {
                        fputs(string, fp2);
                }
        }
        fclose(fp1);
        fclose(fp2);
        remove(path);
        rename(temp, path);
}
/*Checks if the given word exists in the Dictionary using a LINKED QUEUE*/
int Dictionary(char word[width])
{
  FILE *dict;
  node *first=NULL;
  dict=fopen("Dictionary.txt", "r");
  char right[width];
  int i=0;
  for(i=0; i<Queue_Size; i++)
  {
  	fscanf(dict, "%s", right);
  	ENQUEUE(&first, right);
  }

  int flag=0;
  node *temp=first;
  while((!feof(dict))&&(temp!=NULL))
  {
  	if(!strcmp(word, temp->word))
  	{
  		fclose(dict);
  		DEL_QUEUE(&first);
  		return 1;
  	}

  	else
  	{
  		temp=temp->next;
  		fscanf(dict, "%s", right);
  		DEQUEUE(&first);
  		ENQUEUE(&first, right);
  	}
  }

	while(temp!=NULL)
	{
		if(!strcmp(word, temp->word))
  	{
  		return 1;
  	}
		else
		{
			temp=temp->next;
		}

	}

  fclose(dict);
  return 0;
}
/*Checks for Spelling Errors in a File, NOT CASE-SENSITIVE*/
int Spell_Check(char path[path_size], char wrong[m_words][width])
{
  FILE *dict, *f_check;
  f_check=fopen(path,"r");
  char word[width], temp[width], temp1[width];
  int top=0, i=0, line=0;
  int flag=0;
  while(!feof(f_check))
  {
  	fscanf(f_check, "%s", word);
  	strcpy(temp, word);
  	int l=strlen(temp);
  	for(i=0; i<l; i++)
  	{
  		temp[i]=tolower(temp[i]);
  	}
  	if((temp[l-1]=='.')||(temp[l-1]=='?')||(temp[l-1]=='!')||(temp[l-1]==','))
  	{
  		temp[l-1]='\0';
		}
		else if(((temp[l-1]=='s')||(temp[l-1]=='d'))&&(temp[l-2]=='\''))
		{
			temp[l-2]='\0';
		}

  	flag=Dictionary(temp);
  	if(flag==0)
  	{
  		strcpy(wrong[top], word);
  		top++;
  	}
  }
  fclose(f_check);
  return top;
}
