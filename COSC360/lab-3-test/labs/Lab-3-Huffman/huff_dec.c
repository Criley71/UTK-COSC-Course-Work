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
UNIX>*/

typedef struct node
{
  struct node *left;  // read 0
  struct node *right; // read 1
  char *val;          // decoded value
} *Node;              // only need 1 val since we will just follow path
// ie 1101 and 1100 both will be at 110 but one will point right and the other left
// think having 2 values is just more confusing and have to keep track of last bit read
// it will use more space like this but i dont care ( •_•)>⌐■-■  (⌐■_■)

Node new_node()
{
  Node node; //makes a new node with NULL vals for left, right and val and returns node.
  node = malloc(sizeof(struct node));//will be used when reading in the coded file
  node->left = NULL;
  node->right = NULL;
  node->val = NULL;
  return node;
}

void delete_nodes(Node node) //recursive tree deletion
{
  if (node == NULL)
  {
    return;//means we reached the end of the branch of nodes.
  } //unwinds checking for right nodes
  delete_nodes(node->left); // recursivly call down the left, then right
  delete_nodes(node->right);
  free(node->val); // delete the stored val
  free(node);      // delete the node
}

int main(int argc, char *argv[])
{
  int curr_buff, file_index; //curr_buff is the index of the buffer, while file_index is position in file
  char buffer[BUFFER_LIMIT]; // set buffer to be size of buffer limit
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
  Node curr_node;
  while (!(feof(decode_file)))
  {
    curr_buff = 0; // reset the buffer to 0 as we only check if each input is less than limit
    while (curr_buff < BUFFER_LIMIT-1)
    {
      file_index = fgetc(decode_file);
      if (file_index == EOF && curr_buff != 0) // if we reach end of file but are in the encoded value then error
      {
        fprintf(stderr, "file not formatted correctly\n");
        delete_nodes(head);
        fclose(decode_file);
        return (-1);
      }
      if (file_index == EOF && curr_buff == 0)
      {
        break; // we reached end of the file
      }
      buffer[curr_buff] = file_index;
      curr_buff += 1;
      if (file_index == '\0')
      {
        break; // hit null character break out and go to decoded value
      }
    }
    curr_node = head;
    while (!(feof(decode_file)))
    {
      file_index = fgetc(decode_file); // if we read 0 then check if left node exists and create if not
      if (file_index == '0')           // then traverse to the left node
      {
        if (curr_node->left == NULL)
        {
          curr_node->left = new_node();
        }
        curr_node = curr_node->left;
      }
      else if (file_index == '1')
      { // if we read 1 then check if right node exists and create if not
        if (curr_node->right == NULL)
        { // then traverse to the right node
          curr_node->right = new_node();
        }
        curr_node = curr_node->right;
      }
      else if (file_index == '\0')
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

  int file_input_size;
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
    fprintf(stderr, "Error: file is not the correct size.\n");
    delete_nodes(head);
    fclose(message_file);
    return (-1);
  }
  fseek(message_file, -4, SEEK_END);           // read 4 bytes before the end of the file
  fread(&file_input_size, 4, 1, message_file); // fread(input var, element size, element count, file name)
  // check that sizes match
  if (ceil(file_input_size / 8.0) + 4 != file_index)
  {
    fprintf(stderr, "Error: Total bits = %d, but file's size is %d\n", file_input_size, file_index);
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
          fprintf(stderr, "Unrecognized bits\n");
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
          fprintf(stderr, "Unrecognized bits\n");
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