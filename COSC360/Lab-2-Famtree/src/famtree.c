/*Connor Riley
2/9/24
Lab-2-Famtree
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fields.h"
#include "dllist.h"
#include "jrb.h"
#include "jval.h"

// person struct holds the relations of person and will
// be the value in the rb tree, with name as key
typedef struct person
{
  char *name;
  char sex;
  struct person *father;
  struct person *mother;
  Dllist children; // list of children
  int visited;     // used in dfs
  int printed;     // used in print_check
  int num_parents; // used in topological sort
} Person;

/*kind of person is used to determine what info we are reading in.
 *we will look at the first word on each line and return a number
 *depending on what the 1st word is, PERSON, SEX, MOTHER_OF, etc...
 */
int kind_of_person(char *line_read)
{
  if (strcmp(line_read, "PERSON") == 0) // 1=person
  {
    return 1;
  }
  if (strcmp(line_read, "SEX") == 0) // 2=sex
  {
    return 2;
  }
  if (strcmp(line_read, "FATHER") == 0) // 3 = father
  {
    return 3;
  }
  if (strcmp(line_read, "MOTHER") == 0) // 4 = mother
  {
    return 4;
  }
  if (strcmp(line_read, "FATHER_OF") == 0) // 5 = father_of
  {
    return 5;
  }
  if (strcmp(line_read, "MOTHER_OF") == 0) // 6 = mother_of
  {
    return 6;
  }
  return -1; // -1 means none of these were read and something is really messed up then
}
/*is_in_children is used to determine if the child is already in the children list, such as
 * a person being read for father and then read as father_of could lead to being doubled in the list
 * potential parent is parent, potential child is child and traverse the children list,
 * uses strcmp between the children name and name passed into the function
 */
int is_in_children(Dllist tmp_list_ptr, Person *potential_parent, Person *potential_child)
{
  dll_traverse(tmp_list_ptr, potential_parent->children)
  {
    Person *t = tmp_list_ptr->val.v;
    if (strcmp(potential_child->name, t->name) == 0)
    {
      return 0; // return 0 if child exists
    }
  }
  return 1; // 1 if child does not exist in child list
}
/* dfs function to determine if their is a cycle in the family
 * if we get to a node that has been visited then there is a cycle
 * we reset the visited to 0 as we unwind. if stderr if we get
 * a cyclem
 */
int dfs(Person *p)
{
  Dllist dfs_l_tmp;
  if (p->visited == 1)
  {
    fprintf(stderr, "Bad input -- cycle in specification\n");
    exit(0);
  }
  p->visited = 1;
  dll_traverse(dfs_l_tmp, p->children)
  {
    Person *q;
    q = (Person *)dfs_l_tmp->val.v;
    dfs(q);
  }
  p->visited = 0;
  return 0;
}
/***
 * use check print to determine if everyone has printed in the tree
 */
int check_print(JRB tree)
{
  JRB tmp;
  jrb_traverse(tmp, tree)
  {
    Person *p = (Person *)tmp->val.v;
    if (p->printed == 0)
    {
      return 0; // if not all printed
    }
  }
  return 1; // if all printed
}
int main()
{
  Person *p, *q;               // current person is p, testing person is q
  JRB people, find, tmp, tmp2; // people is the family tree, find is used to determine if
                               // someone is already in the tree. tmp and tmp2 are used in traverse
  IS is;                       // input struct
  int nsize;                   // size of name
  Dllist tmp_l, l;             // used as tmp lists for traverse
  char *name_in;               // used to store name input each loop

  is = new_inputstruct(NULL); // get_line
  people = make_jrb();        // jrb tree

  while (get_line(is) >= 0)
  {
    // while we read in lines and that there are words on the line
    if (is->NF > 0)
    {
      for (int i = 0; i < is->NF; i++)
      {
        if (i == 0)
        {
          int type_of_person = kind_of_person(is->fields[0]);
          if (type_of_person == 1)
          { // we read in person
            nsize = strlen(is->fields[1]);
            for (int i = 2; i < is->NF; i++)
            {
              nsize += (strlen(is->fields[i]) + 1);             // start to get size of name
            }                                                   // dont want to allocate whole person node since they may already exist
            name_in = (char *)malloc(sizeof(char) * nsize + 1); // get size of the name, including white space
            strcpy(name_in, is->fields[1]);
            nsize = (strlen(is->fields[1]));
            for (i = 2; i < is->NF; i++)
            {
              name_in[nsize] = ' ';
              strcpy(name_in + nsize + 1, is->fields[i]);
              nsize += strlen(name_in + nsize);
            }

            find = jrb_find_str(people, name_in); // check if name is in jrb tree
            if (find == NULL)
            {                             // if not malloc person and insert into the tree
              p = malloc(sizeof(Person)); // initializes the person with blank data
              p->name = name_in;
              p->sex = '?';
              p->father = NULL;
              p->mother = NULL;
              p->children = new_dllist();
              p->printed = 0;
              p->visited = 0;
              p->num_parents = 0;
              jrb_insert_str(people, p->name, new_jval_v((void *)p)); // add person to tree
            }
            else
            {
              p = find->val.v; // else we set p to the node
            }
          }
          if (type_of_person == 2) // if we read in sex, we know that p is the  person its referring to so we can
          {
            if (p->sex != is->fields[1][0] && p->sex != '?') // error check
            {
              fprintf(stderr, "Bad input - sex mismatch on line %d\n", is->line);
              exit(0);
            } // easily set the sex for the person
            p->sex = is->fields[1][0];
          }
          if (type_of_person == 3) // FATHER (NOT father_of)
          {
            // do same thing for name as we did for
            // person name, but we will store it in q
            // q is father, p is chhild
            nsize = strlen(is->fields[1]);
            for (int i = 2; i < is->NF; i++)
            {
              nsize += (strlen(is->fields[i]) + 1);
            }
            name_in = (char *)malloc(sizeof(char) * nsize + 1);
            strcpy(name_in, is->fields[1]);
            nsize = (strlen(is->fields[1]));
            for (i = 2; i < is->NF; i++)
            {
              name_in[nsize] = ' ';
              strcpy(name_in + nsize + 1, is->fields[i]);
              nsize += strlen(name_in + nsize);
            }
            find = jrb_find_str(people, name_in); // search tree for father
            if (find == NULL)                     // means father is not in tree
            {
              q = malloc(sizeof(Person));
              q->name = name_in;
              q->sex = 'M';
              p->father = q; // this sets p as child and q as father
              q->mother = NULL;
              q->children = new_dllist();
              q->printed = 0;
              q->visited = 0;
              q->num_parents = 0;
              if (p->num_parents <= 1)
              {
                p->num_parents += 1;
              }
              dll_append(q->children, new_jval_v((void *)p));         // add child to father
              jrb_insert_str(people, q->name, new_jval_v((void *)q)); // add father to tree
              // printf("test father didnt exist\n");
            }
            else
            {                          // means the father was already in the tree
              Person *t = find->val.v; // set t to the father
              if (p->father == NULL)
              {
                p->num_parents += 1;
              }
              if (p->father != NULL && p->father != t) // make sure we arent reassigning the father
              {
                fprintf(stderr, "Bad input -- child with two fathers on line %d\n", is->line);
                exit(0);
              }
              p->father = find->val.v; // set the persons father to the value we found
              // determine if the children is already in the list
              if (is_in_children(tmp_l, t, p) == 1)
              {
                dll_append(t->children, new_jval_v((void *)p));
              }
              if (t->sex != 'M' && t->sex != '?') // make sure the father doesnt have a sex that isnt 'M' or NULL
              {
                fprintf(stderr, "Bad input - sex mismatch on line %d\n", is->line);
              }
              t->sex = 'M'; // set father sex to M
            }

            //   printf("test1\n");
          }
          if (type_of_person == 4) // MOTHER (NOT mother_of)
          {
            // do same thing for name as we did for
            // person name, but we will store it in q
            // we may need to make a link between p and q

            nsize = strlen(is->fields[1]);
            for (int i = 2; i < is->NF; i++)
            {
              nsize += (strlen(is->fields[i]) + 1);
            }
            name_in = (char *)malloc(sizeof(char) * nsize + 1);
            strcpy(name_in, is->fields[1]);
            nsize = (strlen(is->fields[1]));
            for (i = 2; i < is->NF; i++)
            {
              name_in[nsize] = ' ';
              strcpy(name_in + nsize + 1, is->fields[i]);
              nsize += strlen(name_in + nsize);
            }
            find = jrb_find_str(people, name_in); // determine if the mother is in the tree
            if (find == NULL)
            {
              q = malloc(sizeof(Person));
              q->name = name_in;
              q->sex = 'F';
              q->father = NULL;
              p->mother = q; // sets p's mother to q
              q->children = new_dllist();
              q->printed = 0;
              q->visited = 0;
              q->num_parents = 0;
              if (p->num_parents <= 1)
              {
                p->num_parents += 1;
              }
              dll_append(q->children, new_jval_v((void *)p));         // add child to mother list
              jrb_insert_str(people, q->name, new_jval_v((void *)q)); // add mother to the tree
            }
            else
            { // if mother existed then we make sure that we arent reassigning mother or sex of
              // the mom. add the child to mother list
              Person *t = find->val.v;
              if (p->mother == NULL)
              {
                p->num_parents += 1;
              }
              if (p->mother != NULL && p->mother != t)
              {
                fprintf(stderr, "Bad input -- child with two mothers on line %d\n", is->line);
              }
              p->mother = find->val.v;
              if (is_in_children(tmp_l, t, p) == 1)
              {
                dll_append(t->children, new_jval_v((void *)p));
              }
              if (t->sex != 'F' && t->sex != '?')
              {
                fprintf(stderr, "Bad input - sex mismatch on line %d\n", is->line);
              }
              t->sex = 'F';
            }
          }
          // reads in  to p the person then
          // will read father_of so p is the father
          // q will be the child.
          // go to q and change father
          // in p add q to the children dllist
          if (type_of_person == 5) // father_of
          {
            nsize = strlen(is->fields[1]); // read in the father of name and determine if its already in the tree
            for (int i = 2; i < is->NF; i++)
            {
              nsize += (strlen(is->fields[i]) + 1);
            }
            name_in = (char *)malloc(sizeof(char) * nsize + 1);
            strcpy(name_in, is->fields[1]);
            nsize = (strlen(is->fields[1]));
            for (i = 2; i < is->NF; i++)
            {
              name_in[nsize] = ' ';
              strcpy(name_in + nsize + 1, is->fields[i]);
              nsize += strlen(name_in + nsize);
            }

            find = jrb_find_str(people, name_in);
            if (find == NULL)
            {                             // if the person wasnt in the tree create them, set there sex to m, and add child to children list
              q = malloc(sizeof(Person)); // then add the person to the tree
              // q is the child
              // p is the parent
              q->name = name_in;
              q->sex = '?';
              q->father = p;
              q->mother = NULL;
              q->children = new_dllist();

              if (p->sex != 'M' && p->sex != '?')
              {
                fprintf(stderr, "Bad input - sex mismatch on line %d\n", is->line);
              }
              p->sex = 'M';
              q->printed = 0;
              q->visited = 0;
              q->num_parents = 1;
              dll_append(p->children, new_jval_v((void *)q));
              jrb_insert_str(people, q->name, new_jval_v((void *)q));
            }
            else
            { // if the person was already in the tree, then check if they are
              // already in the children list and add them if so.
              Person *tmp;
              tmp = find->val.v;
              if (is_in_children(tmp_l, p, tmp) == 1)
              {
                dll_append(p->children, new_jval_v((void *)tmp));
                if (tmp->father == NULL)
                {
                  tmp->num_parents++;
                }
                tmp->father = p;
              }
              else if (tmp->father == NULL)
              {
                if (tmp->father == NULL)
                {
                  tmp->num_parents++;
                }
                tmp->father = p;
              }
              if (p->sex != 'M' && p->sex != '?')
              {
                fprintf(stderr, "Bad input - sex mismatch on line %d\n", is->line);
              }
              p->sex = 'M';
            }
          }
          // reads in  to p the person then
          // will read mother_of so p is the father
          // q will be the child.
          // go to q and change father
          // in p add q to the children dllist
          if (type_of_person == 6) // mother_of
          {                        // read in the mother of and determine if the child is in the tree
            nsize = strlen(is->fields[1]);
            for (int i = 2; i < is->NF; i++)
            {
              nsize += (strlen(is->fields[i]) + 1);
            }
            name_in = (char *)malloc(sizeof(char) * nsize + 1);
            strcpy(name_in, is->fields[1]);
            nsize = (strlen(is->fields[1]));
            for (i = 2; i < is->NF; i++)
            {
              name_in[nsize] = ' ';
              strcpy(name_in + nsize + 1, is->fields[i]);
              nsize += strlen(name_in + nsize);
            }
            find = jrb_find_str(people, name_in);
            if (find == NULL)
            {                             // set the child to default values and set the mom sex to f, and insert the
              q = malloc(sizeof(Person)); // child into the tree
              // q is the child
              // p is the parent
              q->name = name_in;
              q->sex = '?';
              q->father = NULL;
              q->mother = p;
              q->children = new_dllist();
              if (p->sex != 'F' && p->sex != '?')
              {
                fprintf(stderr, "Bad input - sex mismatch on line %d\n", is->line);
              }
              p->sex = 'F';
              q->printed = 0;
              q->visited = 0;
              q->num_parents = 1;
              dll_append(p->children, new_jval_v((void *)q));
              jrb_insert_str(people, q->name, new_jval_v((void *)q));
            }
            else
            {
              // tmp is child
              // p is mom
              Person *tmp; // determine if the child is already in the children list
              tmp = find->val.v;
              if (is_in_children(tmp_l, p, tmp) == 1)
              {
                dll_append(p->children, new_jval_v((void *)tmp)); // add child to lsit
                tmp->mother = p;
                tmp->num_parents += 1;
              }
              else if (tmp->mother == NULL)
              { // set childs mother and increase the number of parents
                tmp->mother = p;
                tmp->num_parents += 1;
              }
              p->sex = 'F'; // set mom sex to f
            }
          }
        }
      }
    }
  }
  // go through the tree calling dfs on each node to determine if cycles exist
  jrb_traverse(tmp, people)
  {
    p = (Person *)tmp->val.v;
    dfs(p);
  }
  // while not everyone is printed we will loop printing a new person
  // we want to print the people who have no parents or if their
  //  parents have been printed.
  while (check_print(people) == 0)
  {
    jrb_traverse(tmp, people)
    {
      Person *p = (Person *)tmp->val.v;
      if (p->printed == 0 && p->num_parents == 0)
      {
        int children_print = 0;
        printf("%s\n", p->name);
        if (p->sex != '?')
        {
          if (p->sex == 'M')
          {
            printf("  Sex: Male\n");
          }
          else
          {
            printf("  Sex: Female\n");
          }
        }
        else
        {
          printf("  Sex: Unknown\n");
        }
        if (p->father != NULL)
        {
          printf("  Father: %s\n", p->father->name);
        }
        else
        {
          printf("  Father: Unknown\n");
        }
        if (p->mother != NULL)
        {
          printf("  Mother: %s\n", p->mother->name);
        }
        else
        {
          printf("  Mother: Unknown\n");
        }
        if (p->children != NULL)
        {
          dll_traverse(tmp_l, p->children)
          {
            if (children_print == 0)
            {
              printf("  Children:\n");
            }
            Person *t = tmp_l->val.v;
            printf("    %s\n", t->name);
            children_print = 1;
          }
        }
        if (children_print == 0)
        {
          printf("  Children: None\n");
        }
        printf("\n");
        p->printed = 1;
        dll_traverse(tmp_l, p->children)
        {
          Person *c = (Person *)tmp_l->val.v;
          c->num_parents -= 1; // go through children list to decrease the childs
                               // parent count
        }

      }
    }
  }
  jrb_traverse(tmp, people)
  {
    Person *p = (Person *)tmp->val.v;
    free_dllist(p->children); //delete each child list
  }
  jrb_free_tree(people); //delete tree
  free(name_in); // delete name in
  jettison_inputstruct(is);// delete input struct

  free(p); //delete person pointer
}//i know that valgrind says i am losing memory and im 99% sure its from the name_in 
// variable. When i free it the gradescripts begin to seg fault, I have had issues with
// the gradescripts this lab where the gradescript would seg fault when running the gradescript 
// program but not if i manually run the example. -\_(^_^)_/-
