#if 0
Parallelogram Madness
Write a program that accepts as input from the user 4 coordinates, and gives as output, “The coordinates form the vertices of a parallelogram” if the coordinates form a parallelogram, and if not, the output should be, “The coordinates do not form the vertices of a parallelogram.” 

Note: you must specify an order in which the coordinates need to be taken so that they form a parallelogram. 

Sample input: 
(0,0), (0,1), (1,0), (1,1)

Sample output: 
The coordinates form a parallelogram in the following order: (0,0), (0,1), (1,1) and (1,0)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX_VERT 4
#define MAX_EDGES 6
typedef struct vertice_tag
{
int x,y;
} vertice;
typedef struct length_tag
{
vertice first, second;
double len;
} lenOfVert;
typedef struct pair_tag
{
    lenOfVert *vert[2];
} pair;
#define sqr(x) (x)*(x)
#define SWAP(x, vect)   \
do                      \
{                       \
lenOfVert *temp;        \
temp = vect[x];         \
vect[x] = vect[x+1];    \
vect[x+1] = temp;       \
}                       \
while(0)
#define IF_MATCH(p1,p2) ((p1.x==p2.x) && (p1.y == p2.y))
double getLength(vertice x, vertice y);
void allocLengths(int i, int j, lenOfVert **vect, vertice input[MAX_VERT]);
int check4Points(vertice input[MAX_VERT]);
int doTheyTouch(lenOfVert *line1, lenOfVert *line2);
void printVertex(vertice point);
int ifMidMatches(pair *diagnol);
void Print4Points(vertice final[MAX_VERT], vertice input[MAX_VERT]);
void Assign4Points(pair pair_lines[MAX_EDGES/2], vertice final[MAX_VERT]);
int main(int argc, char **argv)
{
    //vertice input[MAX_VERT]={{0,0},{1,2},{2,0},{3,2}};
    vertice input[MAX_VERT]={{0,0},{1,1},{0,1},{1,0}};
    vertice final[MAX_VERT];

    // vertice input[MAX_VERT]={{0,0},{1,2},{2,0},{1,2}};
    lenOfVert *length[MAX_EDGES];
    pair pair_lines[MAX_EDGES/2];
    int pairIdx=0;
    int i,j;
    
    if (check4Points(input))
    {
        printf("Not a valid 4 Points \n");
        return 0;
    }
    for(i=0;i<MAX_VERT;i++)
    {
        allocLengths(i, (i+1)%MAX_VERT, &length[i], input);
    }

    /* Find lengths of diagnols */
    allocLengths(i%MAX_VERT, (i+2)%MAX_VERT, &length[i], input);
    ++i;
    allocLengths(i%MAX_VERT, (i+2)%MAX_VERT, &length[i], input);

    /* Sort the lengths */
    for(i=0;i<MAX_EDGES;i++)
    {
        for(j=0;j<(MAX_EDGES-(i+1));j++)
        {
            if (length[j]->len>length[j+1]->len)
            SWAP(j,length);
        }
    }
#if 0
    for(i=0;i<MAX_EDGES;i++) printf("%f ",length[i]->len);
    printf("\n");
#endif
    /* Find pairs of same lengths */
    for(i=0;i<MAX_EDGES;i++)
    {
        for(j=i+1;j<MAX_EDGES;j++)
        {
            if ((length[i]->len == length[j]->len) && !(doTheyTouch(length[i], length[j])))
            {
                pair_lines[pairIdx].vert[0] = length[i];
                pair_lines[pairIdx].vert[1] = length[j];
                pairIdx++;
            }
        }
    }
    //printf("No of Pairs=%d\n",pairIdx);
    if (pairIdx>2)
    {
        /* This is a Square */
        //printf("This is a Square \n");
        /* Hence eliminate the Diagnol pair */
        /* the mid point of diagnol pair is same */
        if (ifMidMatches(&pair_lines[0]))
        {
            pair_lines[0] = pair_lines[1];
            pair_lines[1] = pair_lines[2];
        }
        else if (ifMidMatches(&pair_lines[1]))
        {
            pair_lines[1] = pair_lines[2];
        }
        Assign4Points(pair_lines, final);
        Print4Points(final, input);
    } else
    if (pairIdx==2)
    {
        /* This is a Rectangle or Parellogram */
        //printf("This is a Rectangle or Parellogram \n");
        Assign4Points(pair_lines, final);
        Print4Points(final, input);
    } else
    {
        /* This is a Quadileteral */
        printf("This is a Quadileteral \n");    
    }

}
int ifMidMatches(pair *diagnol)
{
    int ret=0;
    lenOfVert *d1;
    lenOfVert *d2;
    
    d1=diagnol->vert[0];
    d2=diagnol->vert[1];
    double mid_d1_x, mid_d1_y, mid_d2_x, mid_d2_y;
    
    mid_d1_x = (d1->first.x+d1->second.x)/2;
    mid_d1_y = (d1->first.y+d1->second.y)/2;
    mid_d2_x = (d2->first.x+d2->second.x)/2;
    mid_d2_y = (d2->first.y+d2->second.y)/2;
    
    if ((mid_d1_x == mid_d2_x) && (mid_d1_y == mid_d2_y))
    {
        ret=1;
#if 0
        printf("Mid Matched for\n");
        printVertex(d1->first);
        printVertex(d1->second);
        printf("with\n");
        printVertex(d2->first);
        printVertex(d2->second);
        printf("\n");
#endif
    }
    
    return ret;
}
void Print4Points(vertice final[MAX_VERT], vertice input[MAX_VERT])
{
    int i;
    int idx=0;
    int match=0;
    while(match==0 && idx<MAX_VERT)
    {
        if (IF_MATCH(final[idx], input[0]))
            match=1;
        else
            idx++;
    }
    if (IF_MATCH(final[idx+1], input[1]))
    {
        for(i=0;i<MAX_VERT;i++)
            printVertex(final[(idx+i)%MAX_VERT]);
    }
    else
    {
        for(i=0;i<MAX_VERT;i++)
        {
            printVertex(final[idx]);
            if (idx==0) idx = MAX_VERT;
            idx--;
        }
    }
    printf("\n");
}

void Assign4Points(pair pair_lines[MAX_EDGES/2], vertice final[MAX_VERT])
{
    vertice tmp1, tmp2;
    int idx=0;
    final[idx] = pair_lines[0].vert[0]->first;
    idx++;
    final[idx] = pair_lines[0].vert[0]->second;
    idx++;
    //printVertex(pair_lines[0].vert[0]->first);
    //printVertex(pair_lines[0].vert[0]->second);
    if (IF_MATCH(pair_lines[0].vert[0]->second, pair_lines[1].vert[0]->first))
    {
        tmp1 = (pair_lines[1].vert[0]->second);
        if (IF_MATCH(tmp1, pair_lines[0].vert[1]->first))
            tmp2 = (pair_lines[0].vert[1]->second);
        else
            tmp2 = (pair_lines[0].vert[1]->first);
    }
    else if (IF_MATCH(pair_lines[0].vert[0]->second, pair_lines[1].vert[0]->second))
    {
        tmp1 = (pair_lines[1].vert[0]->first);
        if (IF_MATCH(tmp1, pair_lines[0].vert[1]->first))
            tmp2 = (pair_lines[0].vert[1]->second);
        else
            tmp2 = (pair_lines[0].vert[1]->first);
    }
    else if (IF_MATCH(pair_lines[0].vert[0]->second, pair_lines[1].vert[1]->first))
    {
        tmp1 = (pair_lines[1].vert[1]->second);
        if (IF_MATCH(tmp1, pair_lines[0].vert[1]->first))
            tmp2 = (pair_lines[0].vert[1]->second);
        else
            tmp2 = (pair_lines[0].vert[1]->first);
    }
    else if (IF_MATCH(pair_lines[0].vert[0]->second, pair_lines[1].vert[1]->second))
    {
        tmp1 = (pair_lines[1].vert[1]->first);
        if (IF_MATCH(tmp1, pair_lines[0].vert[1]->first))
            tmp2 = (pair_lines[0].vert[1]->second);
        else
            tmp2 = (pair_lines[0].vert[1]->first);
    }
    final[idx] = tmp1;
    idx++;
    final[idx] = tmp2;
    idx++;
    //printVertex(tmp1);
    //printVertex(tmp2);
    //printf("\n");
}
double getLength(vertice x, vertice y)
{
    int x2,y2;
    double len;
    
    x2 = sqr(x.x-y.x);
    y2 = sqr(x.y-y.y);
    len = sqrt(x2+y2);
    //printf("[%d,%d] to [%d,%d] is %f\n",x.x,x.y,y.x,y.y,len);
return len;
}
void allocLengths(int i, int j, lenOfVert **vect, vertice input[MAX_VERT])
{
    *vect = (lenOfVert*)malloc(sizeof(lenOfVert));
    (*vect)->first = input[i];
    (*vect)->second = input[j];
    (*vect)->len= getLength(input[i],input[j]);

}
void printVertex(vertice point)
{
    printf("[%d,%d] ", point.x, point.y);
}
int check4Points(vertice input[MAX_VERT])
{
    int ret=0;
    
    if (
    ((input[0].x==input[1].x) && (input[0].y==input[1].y)) || 
    ((input[0].x==input[2].x) && (input[0].y==input[2].y)) ||
    ((input[0].x==input[3].x) && (input[0].y==input[3].y)) ||
    ((input[1].x==input[2].x) && (input[1].y==input[2].y)) ||
    ((input[1].x==input[3].x) && (input[1].y==input[3].y)) ||
    ((input[2].x==input[3].x) && (input[2].y==input[3].y)))
        ret = 1;
    
    return ret;
}

int doTheyTouch(lenOfVert *line1, lenOfVert *line2)
{
    int ret=0;
    if (IF_MATCH(line1->first, line2->first) || 
        IF_MATCH(line1->first, line2->second) ||
        IF_MATCH(line1->second, line2->first) || 
        IF_MATCH(line1->second, line2->second))
    {
        ret = 1;
    }
    return ret;
}
