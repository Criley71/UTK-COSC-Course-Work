/*Connor Riley
2/4/2024
CS360 Lab-1 Chain Heal
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
/*
The node struct holds info for each player.
holds the name, coordinates, adjacent list.
*/
typedef struct node
{
  char *name;
  int x, y;
  int curr_PP, max_PP;
  struct node *prev;
  int adj_size;
  struct node **adj;
  int visited;    // Once visted set to 1;
  int heal;       // Holds the heal amount for current dfs
  int print_heal; // set to the amount healed if part of the best path
} Node;
/*
the global_info struct holds general information for the program.
holds initial range, jump range, number of jumps, initial power,
power reduction, and dfs related information
*/
typedef struct global_info
{
  int i_range;               // initial range
  int jump_r;                // jump range
  int n_jump;                // number of jumps
  int i_power;               // initial power
  double power_red;          // power reduction multiplier
  int best_healing;          // best overall healing
  double total_poss_healing; // i_power * (1-(p_reduce * (currentj - 1)))
  int total_healing;         // total btween past and curr
  Node **best_path;          // best path of dfs
  int *healing;              // the healing done at each node on best path
  int best_path_length;      // length of the best path

} Global;
/*
finds and returns the node which urgosa is
used when finding where we can start
*/
Node *find_urgosa(Node *node_array, int size)
{
  for (int i = 0; i < size; i++)
  {
    if (strcmp(node_array[i].name, "Urgosa_the_Healing_Shaman") == 0)
    {
      return &node_array[i];
    }
  }
  return NULL;
}
/*
used to find the size of the adjacency list for each node
checks if the distance is within the jump range, makes sure its not
checking if itself is in the distance with strcmp
*/
int find_adj_size(Node *node_array, int jump_range, int size, Node target)
{
  int adj_size = 0;
  for (int i = 0; i < size; i++)
  {
    if (strcmp(node_array[i].name, target.name) == 0)
    {
      continue;
    }
    else
    {
      int x_dist = target.x - node_array[i].x;
      if (x_dist < 0)
      {
        x_dist *= -1;
      }
      x_dist *= x_dist;
      int y_dist = target.y - node_array[i].y;
      if (y_dist < 0)
      {
        y_dist *= -1;
      }
      y_dist *= y_dist;
      double total_distance = sqrt(x_dist + y_dist);
      if (total_distance <= jump_range)
      {
        adj_size += 1;
      }
    }
  }
  return adj_size;
}
/*
much like finding the adj size, this just checks if a node is adjacent to
another node if so, it returns 1 meaning we add it to the adj list
*/
int is_adj(Node *node_array, int jump_range, Node origin, Node target)
{
  if (strcmp(origin.name, target.name) == 0)
  {
    return 0;
  }
  else
  {
    int x_dist = target.x - origin.x;
    if (x_dist < 0)
    {
      x_dist *= -1;
    }
    x_dist *= x_dist;
    int y_dist = target.y - origin.y;
    if (y_dist < 0)
    {
      y_dist *= -1;
    }
    y_dist *= y_dist;
    double total_distance = sqrt(x_dist + y_dist);

    if (total_distance <= jump_range)
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }
}
/*
uses urgosas position as the origin and with the initial range, checks if
the target is a possible starting node for dfs.
*/
int can_start(Node *node_array, int initial_range, Node *origin, Node target)
{
  if (strcmp(origin->name, target.name) == 0)
  {
    return 1; // origin will always be urgosa
  }
  int x_dist = target.x - origin->x;
  if (x_dist < 0)
  {
    x_dist *= -1;
  }
  x_dist *= x_dist;
  int y_dist = target.y - origin->y;
  if (y_dist < 0)
  {
    y_dist *= -1;
  }
  y_dist *= y_dist;
  double total_distance = sqrt(x_dist + y_dist);

  if (total_distance <= initial_range)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}
/*
dfs function, curr_node is the node we are at, current jump is the amount of jumps we are at,
info holds the global information, and the prev node is where we came from.
*/
void dfs(Node *curr_node, int current_jump, Global *info, Node *prev)
{
  int num_jumps = info->n_jump; // amount of jumps allowed

  // if we have visited the node or we are above the max jumps return.
  if (curr_node->visited == 1)
  {
    return;
  }

  if (current_jump > num_jumps)
  {
    return;
  }
  // set the node to visited, set the prev node to prev for backtracking purposes
  // amount_can_heal starts as the total initial power then is calculated to the actual power
  // possible based on the reduction and the amount of jumps done
  curr_node->visited = 1;
  curr_node->prev = prev;
  double Amount_can_heal = info->i_power;
  Amount_can_heal = info->i_power * pow((1 - info->power_red), current_jump - 1);

  // if we are on the first jump we set the amount_can_heal to the inital power and the total healing done
  // on the current dfs path to 0.
  // otherwise we calculate the healing capable based on power reduction
  if (current_jump == 1)
  {
    info->total_poss_healing = info->i_power; // if jump 1 we have max power
    Amount_can_heal = info->total_poss_healing;
    info->total_healing = 0;
  }
  else
  {
    info->total_poss_healing = info->i_power * pow((1 - info->power_red), current_jump - 1);
  }
  // round the amount can heal to an int
  Amount_can_heal = rint(info->total_poss_healing);
  // check if the amount can heal is more than what is possible based on missing pp.
  // if more than the amount can heal is changed to max_pp - curr_pp.
  if ((info->total_poss_healing + curr_node->curr_PP) > curr_node->max_PP)
  {
    Amount_can_heal = curr_node->max_PP - curr_node->curr_PP;
  }
  // checks if it is the startting node if so we set the heal amount to 0
  // otherwise we calculate the total heal based on the previous amount of healing done
  // on the path
  if (curr_node->prev == NULL && current_jump == 1)
  {
    curr_node->print_heal = Amount_can_heal;
    curr_node->heal = Amount_can_heal;
  }
  else
  {
    curr_node->heal = curr_node->prev->heal + Amount_can_heal;
    curr_node->print_heal = Amount_can_heal;
  }
  // set the node pointer to the current node and check if we have a new best healing
  // if so we track back and set the new best healing and the healing done with each node
  Node *p = curr_node;
  if (info->best_healing < curr_node->heal)
  {

    curr_node->print_heal = Amount_can_heal;
    info->best_path_length = current_jump;
    info->best_healing = curr_node->heal;
    int i = 1;

    // loops back like a linked a list
    while (p != NULL)
    {
      info->best_path[current_jump - i] = p;
      info->healing[current_jump - i] = p->print_heal;

      p = p->prev;
      i++;
    }

    p = curr_node;
  }

  info->total_healing = info->total_healing + Amount_can_heal;
  // recursivly call the dfs function for the next node in the adj list
  for (int i = 0; i < curr_node->adj_size; i++)
  {
    dfs(curr_node->adj[i], current_jump + 1, info, curr_node);
  }
  curr_node->visited = 0;
}

int main(int argc, char *argv[])
{
  int initial_range;                     // initial range
  int jump_range;                        // jump range
  int num_jumps;                         // number of jumps
  int initial_power;                     // initial total power
  double power_reduction;                // power reduction multiplier
  char in_name[1024];                    // name allocated to 1024 chars
  int x_in, y_in, curr_pp_in, max_pp_in; // x coordinate, y coordinate, current pp, max pp inputs
  int num_nodes = 0;                     // amount of nodes
  /*the command line arguments stored in variables */
  sscanf(argv[1], "%d", &initial_range);
  sscanf(argv[2], "%d", &jump_range);
  sscanf(argv[3], "%d", &num_jumps);
  sscanf(argv[4], "%d", &initial_power);
  sscanf(argv[5], "%lf", &power_reduction);
  //global info initialization
  Global *global_info_input = malloc(sizeof(Global)); // need to free DONE
  global_info_input->i_range = initial_range;
  global_info_input->jump_r = jump_range;
  global_info_input->n_jump = num_jumps;
  global_info_input->i_power = initial_power;
  global_info_input->power_red = power_reduction;
  global_info_input->total_poss_healing = initial_power;
  global_info_input->best_healing = 0;
  global_info_input->total_healing = 0;
  global_info_input->best_path_length = 0;
  //node array initialization
  Node *all_nodes = NULL;
  int index = 0;
  //loop to fill info for each node from standard input
  while (scanf("%i %i %i %i %s", &x_in, &y_in, &curr_pp_in, &max_pp_in, in_name) == 5)
  {
    ++num_nodes;
    all_nodes = realloc(all_nodes, num_nodes * sizeof(Node)); // need to free DONE
    all_nodes[index].name = strdup(in_name);                  // need to free DONE
    all_nodes[index].x = x_in;
    all_nodes[index].y = y_in;
    all_nodes[index].curr_PP = curr_pp_in;
    all_nodes[index].max_PP = max_pp_in;
    all_nodes[index].visited = 0;
    all_nodes[index].heal = 0;
    all_nodes[index].print_heal = 0;
    index++;
  }

  //allocated the best path and healing array memory
  global_info_input->best_path = malloc(sizeof(Node *) * num_jumps); // need to free DONE
  global_info_input->healing = malloc(sizeof(int) * num_jumps);      // need to free DONE
  global_info_input->best_path[0] = NULL;
  //set prev node 
  for (int i = num_nodes - 1; i >= 0; i--)
  {
    if (i == 0)
    {
      all_nodes[i].prev = NULL;
    }
    else
    {
      all_nodes[i].prev = &all_nodes[i - 1];
    }
  }
  //find and set urgosa's node
  Node *urgosa_node = find_urgosa(all_nodes, num_nodes);

  for (int i = 0; i < num_nodes; i++)
  {
    all_nodes[i].adj_size = find_adj_size(all_nodes, jump_range, num_nodes, all_nodes[i]);
  }
  // allocate memory for adj list
  for (int i = 0; i < num_nodes; i++)
  {
    all_nodes[i].adj = malloc(sizeof(Node *) * all_nodes[i].adj_size); // need to free DONE
  }
  //set adj list based for each node
  for (int i = 0; i < num_nodes; i++)
  {
    int adj_index = 0;
    for (int j = 0; j < num_nodes; j++)
    {
      if (is_adj(all_nodes, jump_range, all_nodes[i], all_nodes[j]) == 1)
      {
        all_nodes[i].adj[adj_index] = &all_nodes[j];
        adj_index += 1;
      }
    }
  }
  // if we can start at a node, call dfs on it.
  for (int i = 0; i < num_nodes; i++)
  {
    if (can_start(all_nodes, initial_range, urgosa_node, all_nodes[i]) == 1)
    {
      Node *starter = &all_nodes[i];
      dfs(starter, 1, global_info_input, NULL);
    }
  }
  // loop through the best path and print the path and healing done at that node
  for (int i = 0; i < global_info_input->best_path_length; i++)
  {
    if (global_info_input->best_path[i] != NULL)
    {
      printf("%s %d\n", global_info_input->best_path[i]->name, global_info_input->healing[i]);
    }
  }
  //print total healing
  printf("Total_Healing %d\n", global_info_input->best_healing);
  //free memory
  free(global_info_input->best_path);
  free(global_info_input->healing);
  free(global_info_input);
  for (int i = 0; i < num_nodes; i++)
  {
    free(all_nodes[i].adj);
  }
  for (int i = 0; i < num_nodes; i++)
  {
    free(all_nodes[i].name);
  }
  free(all_nodes);
//thank you Adam for your help with this lab :)
}
