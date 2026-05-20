#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define BUFFER_LIMIT 10001
/*UNIX> xxd -g 1 data/t-code.txt
00000000: 6f 00 30 30 00 70 00 30 31 30 00 74 68 65 00 30  o.00.p.010.the.0
00000010: 31 31 00 20 00 31 30 00 0a 00 31 31 30 30 00 66  11. .10...1100.f
00000020: 00 31 31 30 31 00 74 00 31 31 31 00              .1101.t.111.
o = 00
p = 010
the = 011
" " = 10
... = \n = 1100
f = 1101
t = 111
UNIX>
typedef struct node
{
  struct node *left_child;
  struct node *right_child;
  char *decoded_val_zero;
  char *decoded_val_one;
} *Node_p;

Node_p create_node()
{
  Node_p new_node;
  new_node = malloc(sizeof(struct node));
  new_node->left_child = NULL;
  new_node->right_child = NULL;
  new_node->decoded_val_zero = NULL;
  new_node->decoded_val_one = NULL;
  return new_node;
}

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    fprintf(stderr, "Incorrect number of args\n Usage: %s code_definition_file input_file\n", argv[1]);
  }

  FILE *decode_file, *input_file;
  Node_p head, curr_node;



  FILE *decode_file, *input_file;
  long int input_size;
  char buffer[BUFFER_LIMIT];
  long int buffer_index;
  long int b;
  long int n;
  long int t;
  char prev;

  decode_file = fopen(argv[1], "r"); // open decode file as read only
  if (decode_file == NULL)
  {
    perror("decode file: ");
    return -1;
  }

  Node_p head = malloc(sizeof(struct node));
  Node_p current_node;
  while (!feof(decode_file)) // read in decode file
  {
    buffer_index = 0;                   // go through the file with index
    while (buffer_index < BUFFER_LIMIT) // if we reach limit then some bs happening
    {
      b = fgetc(decode_file); // b points to current char in the file
      if (b == EOF && buffer_index != 0)
      { // reached end of file but it didnt read a null char
        fprintf(stderr, "error, still reading string but reached end of file\n");
        fclose(decode_file);
        return -1;
      }
      if (b == EOF && buffer_index == 0)
      {
        break; // means we reached end of file and didnt read in anything
      }
      // add char to the buffer array;
      buffer[buffer_index] = b;
      buffer_index++; // increase index
      if (b == '\0')  // if we reach a null char then we can start to add
      {               // the information to the binary tree
        break;
      }
    }

    current_node = head;
    prev = NULL;
    while (!feof(decode_file))
    {
      b=fgetc(decode_file);
      printf("%ld\n", b);
      if (b == '0')
      {
        if (current_node->left_child == NULL)
        {
          current_node->left_child = create_node();
        }
        current_node = current_node->left_child;
        prev = '0';
      }
      else if (b == '1')
      {
        if (current_node->right_child == NULL)
        {
          current_node->right_child = create_node();
        }
        current_node = current_node->right_child;
        prev = '1';
      }
      else if (b == '\0')
      {
        if (prev == '0')
        {
          current_node->decoded_val_zero = malloc(sizeof(char) * (buffer_index + 1));
          strcpy(current_node->decoded_val_zero, buffer);
        }
        if (prev == '1')
        {
          current_node->decoded_val_one = malloc(sizeof(char) * (buffer_index + 1));
          strcpy(current_node->decoded_val_zero, buffer);
          break;
        }
      }
      // add error check here for file formatting
    }
  }
  fclose(decode_file);

  //need to open the next file
  //can determine if less than 4 bytes by fseek and ftell https://www.geeksforgeeks.org/c-program-find-size-file/
  //seek from end-4 to end
  //read in last 4 bytes ceil(input_size/8.0)+4 check if equal to position at seek end (size in bytes)
  //read in file, check if zero then go left if 1 go right, if we hit a null ptr then bad sequence

  input_file = fopen(argv[2], "r");
  if (input_file == NULL)
  {
    perror("input file");
    return -1;
  }

  fseek(input_file, 0, SEEK_END);
  if (n < 4)
  {
    fprintf(stderr, "error: file not right size");
    fclose(input_file);
    return -1;
  }
  fseek(input_file, -4, SEEK_END);
  fread(&input_size, 4, 1, input_file);
  if (ceil(input_size / 8.0) + 4 != n)
  {
    fprintf(stderr, "sizes dont match total bits is %ld, file is %ld\n", input_size, n);
    fclose(input_file);
    return -1;
  }

  fseek(input_file, 0, SEEK_SET);
  current_node = head;
  int last_in = NULL;
  while (1 == 1)
  {
    t = fgetc(input_file);
    if (feof(input_file) || input_size == 0)
    {
      break;
    }
    int i = 0;
    while(i < 8 && input_size != 0){
      if(t == 1){
        if(current_node->right_child != NULL){
          current_node = current_node->right_child;
          last_in = 1;
        }else{
          fprintf(stderr, "not recognized bits");
          fclose(input_file);
          return -1;
        }
      }else if (t==0){
        if(current_node->left_child != NULL){
          current_node = current_node->left_child;
          last_in = 0;
        }else{
          fprintf(stderr, "not recognzide bits");
        }
      }
      if(current_node!=NULL){
        if(last_in == 0){
          printf("%s", current_node->left_child);
          current_node = head;
        }
        else if(last_in == 1){
          printf("%s", current_node->right_child);
          current_node = head;
        }
      }

      i += 1;
      input_size -= 1;
    }
  }

}
  */

typedef struct node
{
  struct node *left;  // read 0
  struct node *right; // read 1
  char *val;          // decoded value
} *Node;              // only need 1 val since we will just follow path
// ie 1101 and 1100 both will be at 110 but one will point right and the other left

Node new_node()
{
  Node node;
  node = malloc(sizeof(struct node));
  node->left = NULL;
  node->right = NULL;
  node->val = NULL;
  return node;
}

void delete_nodes(Node node)
{
  if (node == NULL)
  {
    return;
  }
  delete_nodes(node->left); // recursivly call down the left, then right
  delete_nodes(node->right);
  free(node->val); // delete the stored val
  free(node);      // delete the node
}

int main(int argc, char *argv[])
{
  long int curr_buff, index;
  char buffer[BUFFER_LIMIT];
  if (argc != 3)
  {
    fprintf(stderr, "bad num of args\n");
    return (-1);
  }
  FILE *decode_file;
  decode_file = fopen(argv[1], "r");
  if (decode_file == NULL)
  {
    perror("decode file cant open\n");
    return (-1);
  }
  Node head = malloc(sizeof(struct node));
  Node curr_node = malloc(sizeof(struct node));
  while (!(feof(decode_file)))
  {
    curr_buff = 0; // reset the buffer to 0 as we only check if each input is less than limit
    while (curr_buff < BUFFER_LIMIT - 1)
    {
      index = fgetc(decode_file);
      if (index == EOF && curr_buff != 0) // if we reach end of file but are in the encoded value then error
      {
        fprintf(stderr, "file not formatted correctly\n");
        delete_nodes(head);
        fclose(decode_file);
        return (-1);
      }
      if (index == EOF && curr_buff == 0)
      {
        break; // we reached end of the file
      }
      buffer[curr_buff] = index;
      curr_buff += 1;
      if (index == '\0')
      {
        break; // hit null character break out and go to decoded value
      }
    }
    curr_node = head;
    while (!(feof(decode_file)))
    {
      index = fgetc(decode_file); // if we read 0 then check if left node exists and create if not
      if (index == '0')           // then traverse to the left node
      {
        if (curr_node->left == NULL)
        {
          curr_node->left == new_node();
        }
        curr_node = curr_node->left;
      }
      else if (index == '1')
      { // if we read 1 then check if right node exists and create if not
        if (curr_node->right == NULL)
        { // then traverse to the right node
          curr_node->right == new_node();
        }
        curr_node = curr_node->right;
      }
      else if (index == '\0')
      {                                                            // if we read null character then we are at the location of the value
        curr_node->val = malloc((curr_buff + 1) * (sizeof(char))); // make space for val and then copy over
        strcpy(curr_node->val, buffer);
        break;
      }
      else
      {
        fprintf(stderr, "file not formatted correctly\n");
        fclose(decode_file);
        delete_nodes(head);
        return (-1);
      }
    }
  }
  fclose(decode_file);

  long int file_index;
  long int file_input_size;
  FILE *message_file;
  message_file = fopen(argv[2], "r");
  if (message_file == NULL)
  {
    perror("mesage file cant open\n");
    delete_nodes(head);
    return 0;
  }
  // check that file is the right size
  fseek(message_file, 0, SEEK_END); // send file to end of the message file
  file_index = ftell(message_file);
  if (file_index < 4)
  { // if less than 4 bytes the file is too small
    fprintf(stderr, "File too small");
    delete_nodes(head);
    fclose(message_file);
    return (-1);
  }
  fseek(message_file, -4, SEEK_END);           // read 4 bytes before the end of the file
  fread(&file_input_size, 4, 1, message_file); // fread(input var, element size, element count, file name)
  // check that sizes match
  if (ceil(file_input_size / 8.0) + 4 != file_index)
  {
    fprintf(stderr, "error sizes dont match\n");
    delete_nodes(head);
    fclose(message_file);
    return (-1);
  }
  fseek(message_file, 0, SEEK_SET);
  curr_node = head;
  while (1 == 1)
  {
    file_index = fgetc(message_file);
    if (file_input_size == 0 || feof(message_file)) // if file size is 0 or we are at end then break
    {
      break;
    }
    for (int i = 0; file_input_size != 0 && i < 8; i++)
    {
      if (file_index & (1 << i)) // read in a 1 by left shifting and testing if 1
      {
        if (curr_node->right != NULL)
        {
          curr_node = curr_node->right; //go right if exist
        }
        else
        {
          fprintf(stderr, "not recogized pattern\n");
          delete_nodes(head);
          fclose(message_file);
          return (-1);
        }
      }
      else //read in 0
      {
        if (curr_node->left != NULL)
        {
          curr_node = curr_node->left; //go left if exist
        }
        else
        {
          fprintf(stderr, "not recogized pattern\n");
          delete_nodes(head);
          fclose(message_file);
          return (-1);
        }
      }
      if (curr_node->val != NULL)
      {
        printf("%s", curr_node->val); //print value and reset back to head
        curr_node = head;
      }
      file_input_size -= 1;
    }
  }
  fclose(message_file);
  delete_nodes(head);
  return(0);
}