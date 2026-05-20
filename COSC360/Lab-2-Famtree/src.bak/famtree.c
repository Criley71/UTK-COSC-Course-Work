#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fields.h"
#include "dllist.h"
#include "jrb.h"
#include "jval.h"

typedef struct person
{
        char *name;
        char sex;
        struct person *father;
        struct person *mother;
        Dllist children;
        int visited;
        int printed;
} Person;
// use getline to find what data type to input- getline returns nf, loop through, nf[0] should be type of person
// 1st word is person, mother of, etc
// make function to figure out what kind it is
// check if person exists, if so make connection,
// or make person
// initialize person with all unknowns and children with none
// sex can be identified via mother_of or father_of
// the red-black tree is more of a doubly linked list that is balanced.
// look at rb-notes for examples on how to take input and distribute to tree
// error check
/*
  Specifying two fathers or two mothers.
  Specifying a father when the person is female.
  Specifying a mother when the person is male.
  Specifying that a person is female when the person is already known to be male.
  Specifying that a person is male when the person is already known to be female.
  Having a cycle in the specification. */

int kind_of_person(char *line_read)
{
        if (strcmp(line_read, "PERSON") == 0)
        {
                return 1;
        }
        if (strcmp(line_read, "SEX") == 0)
        {
                return 2;
        }
        if (strcmp(line_read, "FATHER") == 0)
        {
                return 3;
        }
        if (strcmp(line_read, "MOTHER") == 0)
        {
                return 4;
        }
        if (strcmp(line_read, "FATHER_OF") == 0)
        {
                return 5;
        }
        if (strcmp(line_read, "MOTHER_OF") == 0)
        {
                return 6;
        }
        return -1;
}
int is_in_children(Dllist tmp_list_ptr, Person *potential_parent, Person *potential_child)
{

        dll_traverse(tmp_list_ptr, potential_parent->children)
        {
                Person *t = tmp_list_ptr->val.v;
                printf("checking if %s is in child array of %s\n", potential_child->name, potential_parent->name);
                if (strcmp(potential_child->name, t->name) == 0)
                {
                        return 0; // return 0 if child exists
                }
        }
        return 1; // 1 if child does not exist in child list
}



int main()
{
        //return(0);
        Person *p, *q;
        JRB people, find, tmp, tmp2;
        IS is;
        int nsize;
        Dllist mothers;
        Dllist fathers;
        Dllist children;
        Dllist tmp_l, l;
        char *name_in;

        is = new_inputstruct(NULL);
        people = make_jrb();
        mothers = new_dllist();
        fathers = new_dllist();
        children = new_dllist();

        while (get_line(is) >= 0)
        {

                if (is->NF > 0)
                {
                        // printf("%d ", is->line);
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
                                                        nsize += (strlen(is->fields[i]) + 1);       // start to get size of name
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
                                                // printf("%s\n", name_in);
                                                find = jrb_find_str(people, name_in); // check if name is in jrb tree
                                                if (find == NULL)
                                                { // if not malloc person and insert into the tree
                                                        // printf("MAKING PERSON\n");
                                                        p = malloc(sizeof(Person));
                                                        p->name = name_in;
                                                        p->sex = '?';
                                                        p->father = NULL;
                                                        p->mother = NULL;
                                                        p->children = new_dllist();
                                                        // printf("made children of %s at %p\n", p->name, p->children);
                                                        p->printed = 0;
                                                        p->visited = 0;
                                                        jrb_insert_str(people, p->name, new_jval_v((void *)p));
                                                        // printf("%s\n", p->name);
                                                }
                                                else
                                                {
                                                        p = find->val.v;
                                                        //  printf("please work\n");
                                                }
                                                //  printf("test\n");
                                        }
                                        if (type_of_person == 2) // if we read in sex, we know that p is the  person its referring to so we can
                                        {                        // easily set the sex for the person
                                                p->sex = is->fields[1][0];
                                                // printf("%s, %d, %d, %d\n",is->fields[1][0], sizeof(is->fields[1][0]), sizeof(p->sex), sizeof(&p->sex));//sex, 8, 1, 8
                                                // printf("%c\n", p->sex);
                                        }
                                        if (type_of_person == 3) // FATHER (NOT father_of)
                                        {
                                                // do same thing for name as we did for
                                                // person name, but we will store it in q
                                                // we may need to make a link between p and q

                                                // do  i need to free or reset in_name, no i didnt in the loop so
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
                                                {
                                                        q = malloc(sizeof(Person));
                                                        q->name = name_in;
                                                        q->sex = 'M';
                                                        p->father = q; // this sets p as child and q as father
                                                        q->mother = NULL;
                                                        q->children = new_dllist();
                                                        q->printed = 0;
                                                        q->visited = 0;
                                                        dll_append(q->children, new_jval_v((void *)p));
                                                        jrb_insert_str(people, q->name, new_jval_v((void *)q));
                                                        // printf("test father didnt exist\n");
                                                }
                                                else
                                                {
                                                        p->father = find->val.v;
                                                        Person *t = find->val.v;
                                                        // printf("HERE-> %d\n", is_in_children(tmp_l, t, p));
                                                        if (is_in_children(tmp_l, t, p) == 1)
                                                        {
                                                                // printf("test father existed\n");
                                                                dll_append(t->children, new_jval_v((void *)p));
                                                        }
                                                        t->sex = 'M';
                                                }
                                                //   printf("test1\n");
                                        }
                                        if (type_of_person == 4) // MOTHER (NOT mother_of)
                                        {
                                                // do same thing for name as we did for
                                                // person name, but we will store it in q
                                                // we may need to make a link between p and q

                                                // do  i need to free or reset in_name, no dont think so, i didnt in the loop
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
                                                {
                                                        q = malloc(sizeof(Person));
                                                        q->name = name_in;
                                                        q->sex = 'F';
                                                        q->father = NULL;
                                                        p->mother = q; // sets p's mother to q
                                                        q->children = new_dllist();
                                                        q->printed = 0;
                                                        q->visited = 0;
                                                        dll_append(q->children, new_jval_v((void *)p));
                                                        jrb_insert_str(people, q->name, new_jval_v((void *)q));
                                                }
                                                else
                                                {
                                                        p->mother = find->val.v;
                                                        Person *t = find->val.v;
                                                        if (is_in_children(tmp_l, t, p) == 1)
                                                        {
                                                                dll_append(t->children, new_jval_v((void *)p));
                                                        }
                                                        t->sex = 'F';
                                                }
                                                //   printf("test2\n");
                                        }
                                        // reads in  to p the person then
                                        // will read father_of so p is the father
                                        // q will be the child.
                                        // go to q and change father
                                        // in p add q to the children dllist
                                        if (type_of_person == 5) // father_of
                                        {
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
                                                // printf(" current person %s is father_of = %s\n", p->name, name_in);
                                                find = jrb_find_str(people, name_in);
                                                if (find == NULL)
                                                {
                                                        q = malloc(sizeof(Person));
                                                        // q is the child
                                                        // p is the parent
                                                        q->name = name_in;
                                                        q->sex = NULL;
                                                        q->father = p;
                                                        q->mother = NULL;
                                                        q->children = new_dllist();
                                                        // printf("made %s father_of %s %p\n",p->name,q->name, q->children);
                                                        p->sex = 'M';
                                                        q->printed = 0;
                                                        q->visited = 0;
                                                        dll_append(p->children, new_jval_v((void *)q));
                                                        jrb_insert_str(people, q->name, new_jval_v((void *)q));
                                                        // printf(" set %s as father_of %s\n", p->name, q->name);
                                                }
                                                else
                                                {
                                                        Person *tmp;
                                                        tmp = find->val.v;
                                                        if (is_in_children(tmp_l, p, tmp) == 1)
                                                        {
                                                                dll_append(p->children, new_jval_v((void *)tmp));
                                                                tmp->father = p;
                                                        }
                                                        else if (tmp->father == NULL)
                                                        {
                                                                tmp->father = p;
                                                        }
                                                        tmp->sex = 'M';
                                                        // printf("set %s as father_of %s\n", p->name, tmp->name);
                                                }
                                                //  printf("test3\n");
                                        }
                                        // reads in  to p the person then
                                        // will read mother_of so p is the father
                                        // q will be the child.
                                        // go to q and change father
                                        // in p add q to the children dllist
                                        if (type_of_person == 6) // mother_of
                                        {
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
                                                // printf(" current person %s is mother_of = %s\n", p->name, name_in);
                                                find = jrb_find_str(people, name_in);
                                                if (find == NULL)
                                                {
                                                        q = malloc(sizeof(Person));
                                                        // q is the child
                                                        // p is the parent
                                                        q->name = name_in;
                                                        q->sex = NULL;
                                                        q->father = NULL;
                                                        q->mother = p;
                                                        q->children = new_dllist();
                                                        // printf("made %s mother_of %s %p\n",p->name,q->name, q->children);
                                                        p->sex = 'F';
                                                        q->printed = 0;
                                                        q->visited = 0;
                                                        dll_append(p->children, new_jval_v((void *)q));
                                                        jrb_insert_str(people, q->name, new_jval_v((void *)q));
                                                        //  printf("%s is mother_of %s\n", p->name, q->name);
                                                }
                                                else
                                                {
                                                        // tmp is child
                                                        // p is mom
                                                        Person *tmp;
                                                        tmp = find->val.v;
                                                        // printf("p is %s\n", p->name);
                                                        // printf("tmp is %s\n", tmp->name);
                                                        // printf("testing if %s is mother of %s\n", tmp->name, p->name);
                                                        // printf("%d\n", is_in_children(tmp_l, p, tmp));
                                                        if (is_in_children(tmp_l, p, tmp) == 1)
                                                        {
                                                                dll_append(p->children, new_jval_v((void *)tmp));
                                                                tmp->mother = p;
                                                        }
                                                        else if (tmp->mother == NULL)
                                                        {
                                                                tmp->mother = p;
                                                        }
                                                        tmp->sex = 'F';
                                                        // printf("%s is mother_of %s\n", p->name, tmp->name);
                                                }
                                                // printf("test4\n");
                                        }
                                        // printf("\n");
                                }

                                // printf("%s", is->fields[i]);
                        }
                        // printf("\n");
                }
        }
        // printf("\n\n\n\n");
        return 0;
        /*
        JRB itr;
        Dllist sorted = new_dllist();
        
        while (jrb_empty(people) == 0)
        {
                itr = jrb_first(people);
                int unchanged = 0;

                do
                {
                        Person *p = (Person *)itr->val.v;
                        itr = jrb_next(itr);
                        if ((p->father == NULL || jrb_find_str(people, p->father->name) == NULL) && (p->mother == NULL || jrb_find_str(people, p->mother->name) == NULL))
                        {
                                dll_append(sorted, new_jval_v((void *)p));
                                jrb_delete_node(jrb_prev(itr));
                                unchanged = 1;
                        }

                        unchanged = 0;
                } while (itr != people);
                if (unchanged == 0)
                {
                        exit(-1);
                }
        }

        dll_traverse(tmp_l, sorted)
        {
                int children_print = 0;
                p = tmp_l->val.v;
                if (p->sex != NULL)
                {
                        if (p->sex == 'M')
                        {
                                printf("   Sex: Male\n");
                        }
                        else
                        {
                                printf("   Sex: Female\n");
                        }
                }
                else
                {
                        printf("   Sex: Unknown\n");
                }
                if (p->father != NULL)
                {
                        printf("   Father: %s\n", p->father->name);
                }
                else
                {
                        printf("   Father: Unknown\n");
                }
                if (p->mother != NULL)
                {
                        printf("   Mother: %s\n", p->mother->name);
                }
                else
                {
                        printf("   Mother: Unknown\n");
                }
                if (p->children != NULL)
                {
                        dll_traverse(tmp_l, p->children)
                        {
                                if (children_print == 0)
                                {
                                        printf("   Children:\n");
                                }
                                Person *t = tmp_l->val.v;
                                printf("      %s\n", t->name);
                                children_print = 1;
                        }
                }
                if (children_print == 0)
                {
                        printf("   Children: None\n");
                }
                printf("\n");
        }
        /*
        jrb_traverse(tmp, people)
        {
                int children_print = 0;
                p = (Person *)tmp->val.v;
                // int dll_size =  malloc(sizeof(dll_empty(mothers)));
                // printf("%d\n", dll_size);
                printf("%s\n", p->name);
                if (p->sex != NULL)
                {
                        if (p->sex == 'M')
                        {
                                printf("   Sex: Male\n");
                        }
                        else
                        {
                                printf("   Sex: Female\n");
                        }
                }
                else
                {
                        printf("   Sex: Unknown\n");
                }
                if (p->father != NULL)
                {
                        printf("   Father: %s\n", p->father->name);
                }
                else
                {
                        printf("   Father: Unknown\n");
                }
                if (p->mother != NULL)
                {
                        printf("   Mother: %s\n", p->mother->name);
                }
                else
                {
                        printf("   Mother: Unknown\n");
                }
                if (p->children != NULL)
                {
                        dll_traverse(tmp_l, p->children)
                        {
                                if (children_print == 0)
                                {
                                        printf("   Children:\n");
                                }
                                Person *t = tmp_l->val.v;
                                printf("      %s\n", t->name);
                                children_print = 1;
                        }
                }
                if (children_print == 0)
                {
                        printf("   Children: None\n");
                }
                printf("\n");
        }
        */
}

/*dfs set as visited traverse to children*/