#include<bits/stdc++.h>
using namespace std;

//node class
struct node{
    vector<int> initial;
    int g_cost, h_cost, f_cost;
    node* parent;

    node(){
        g_cost = 0;
        h_cost = 0;
        f_cost = 0;
        parent = NULL;
    }
};

//operator overloading for priority_queue
struct comp{
    bool operator()(const node* lhs, const node* rhs) const{
        return lhs->f_cost > rhs->f_cost;
    }
};

int N = 9;
vector<int> goal;
vector<node*> visited;
priority_queue<node*, vector<node*>, comp> to_be_explored;

//print board
void print(node* n){
    int dim = sqrt(N);
    int i, j, k = 0;
    for(i = 0; i < dim; ++i){
        for(j = 0; j < dim; ++j){
        	printf("%d ", n->initial[k++]);
		}
        printf("\n");
    }
    printf("\n");
}

//print solution
void path_solution(node* n){
    node* temp = n;
    int i;
    vector<node*> totalpath;	//it's a list for containing all the nodes in the path from the root to the goal
    while(temp != NULL){
        totalpath.push_back(temp);
        temp = temp->parent;
    }

    int sz = totalpath.size();
    //print number of moves required
    printf("Moves: %d\n", sz-1);

    //show the moves one by one
    for(i = sz-1; i >= 0; --i){
    	print(totalpath[i]);
	}
}

//calculate heuristic
int calculate_misplaced_tiles(node* n){
    int cnt = 0;
    int i;
    for(i = 0; i < N; ++i){
    	if(goal[i] != n->initial[i]){
    		++cnt;
		}
	}
    return cnt;
}

//build successor node
node* create_successor(node* state, int pos1, int pos2){
    node* new_state = new node();
    new_state->initial = state->initial;
    swap(new_state->initial[pos1], new_state->initial[pos2]);
    new_state->h_cost = calculate_misplaced_tiles(new_state);
    new_state->g_cost = state->g_cost+1;
    new_state->f_cost = new_state->h_cost + new_state->g_cost;
    new_state->parent = state;

    return new_state;
}

//generate successors
vector<node*> get_successor(node* n){
    int pos, row, col, dim;
    for(int i = 0; i < N; ++i){
        if(n->initial[i] == 0){
            pos = i;
            break;
        }
    }
    dim = sqrt(N);
    row = pos / dim;
    col = pos % dim;

    vector<node*> successors;

    if(col != 0){
    	successors.push_back(create_successor(n, pos, pos-1));
	}
    if(col != dim-1){
    	successors.push_back(create_successor(n, pos, pos+1));
	}
    if(row != 0){
    	successors.push_back(create_successor(n, pos, pos-dim));
	}
    if(row != dim-1){
    	successors.push_back(create_successor(n, pos, pos+dim));
	}
    return successors;
}

//return true if goal
bool is_Goal(node* n){
    return (n->h_cost == 0) ? 1 : 0;
}

bool check_visited(node* n){
    int sz = visited.size(), i, j;
    for(i = 0; i < sz; ++i){
        for(j = 0; j < N; ++j){
            if(n->initial[j] != visited[i]->initial[j]){
            	break;
			} 
        }
        if(j == N){
        	return 1;
		}   
    }
    return 0;
}

void Asearch(node* n){
    n->h_cost = calculate_misplaced_tiles(n);
    n->f_cost = n->h_cost;
    n->parent = NULL;
    to_be_explored.push(n);

    bool flag;
    int tentative_g_cost, sz, i, k;
    node *current, *temp;
    vector<node*> current_successors;
    priority_queue<node*, vector<node*>, comp> pq;

    while(!to_be_explored.empty()){
        current = to_be_explored.top();
        to_be_explored.pop();
        visited.push_back(current);

        if(is_Goal(current)){
            path_solution(current);
            return;
        }

        current_successors.clear();
        current_successors = get_successor(current);

        sz = current_successors.size();
        for(i = 0; i < sz; ++i){
            if(check_visited(current_successors[i])){
            	continue;
			}
            tentative_g_cost = current->g_cost+1;

            while(!pq.empty()){
            	pq.pop();
			}
            while(!to_be_explored.empty()){
                temp = to_be_explored.top();
                to_be_explored.pop();

                flag = 0;
                for(k = 0; k < N; ++k){
                	if(current_successors[i]->initial[k] != temp->initial[k]){
                		break;
					} 
				}
                if(k == N){
                	flag = 1;
				}
                if(flag && tentative_g_cost < temp->g_cost){
                    temp->parent = current;
                    temp->g_cost = tentative_g_cost;
                    temp->f_cost = temp->g_cost + temp->h_cost;
                }
                pq.push(temp);
            }
            if(!flag){
            	pq.push(current_successors[i]);
			} 
            to_be_explored = pq;
        }
    }

    return;
}

int main(){
    int i, x;
    node* newnode = new node();
    
    goal.push_back(1);
    goal.push_back(2);
    goal.push_back(3);
    goal.push_back(4);
    goal.push_back(5);
    goal.push_back(6);
    goal.push_back(7);
    goal.push_back(0);
    goal.push_back(8);
    
    printf("Goal State:\n");
    for(i = 0; i < 3; ++i){
        printf("%d ", goal[i]);
    }
    printf("\n");
    for(i = 3; i < 6; ++i){
        printf("%d ", goal[i]);
    }
    printf("\n");
    for(i = 6; i < N; ++i){
        printf("%d ", goal[i]);
    }
    printf("\n");
	printf("Input initial state:\n");
    for(i = 0; i < N; ++i){
        scanf("%d", &x);
        newnode->initial.push_back(x);
    }

    Asearch(newnode);
}
