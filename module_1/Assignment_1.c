
void main(int argc , char * argv[])
{
  int i,sum=0;

  if(argc < 3)
  {
    printf("Not enough numbers to add up!");
    exit(1);
  }

  for(i=1;i<argc;i++)
    sum = sum + atoi(argv[i]);

  printf("%d",sum);

}
