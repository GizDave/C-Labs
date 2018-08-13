extern FILE *file;
extern struct shell *hanger;

struct node{
	double value;
	struct node *next;
};

struct shell{
	struct node *firstNode;
	struct shell *next;
};