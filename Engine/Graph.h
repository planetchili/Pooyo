#pragma once

//#include <glm.hpp>
#include <vector>
#include <list>

//using glm::vec2;

//class Texture;
//class SpriteBatch;
//class Font;
//class Agent;

class Graph 
{
	public:
		class Edge;
	public:
		class Node
		{
			public:
				// Add the approprate data
				bool visited;
				bool traversed;
				float G_Score;
				float F_Score;
				Node* previousNode;
				typedef		std::vector<Edge*> EdgeList;
				EdgeList	edges;
				vec2 pos;
				Texture* texture;
				
				Node(vec2, Texture*);
				~Node();

				void setTexture(Texture*);

		}; 
		class Edge 
		{ 
			public: 
				Node* Start;
				Node* End;
				float cost;
				
				Texture* texture;
				vec2 centre;
				float length;
				float rotation;

				Edge(Node* start, Node* end, float cost, Texture* texture);

		}; 
	public: 
		Graph();
		void LoadTextures();
		void MakeGrid(vec2 size, vec2 spacing, vec2 startPos);
		virtual ~Graph();
		Node *AddNode(vec2 data, Texture*);
		Node *FindNode(vec2 data); // find node with value
		Node *FindNode(vec2 data, float range); // find node with value 
		void RemoveConnectingEdges(Node *pNode); // remove given edge iterator;
		void RemoveNode(Node *pNode); // remove the given node 
		void ConnectNodesInRange(Node *pNode, float range, Texture* texture);
		void ConnectNodes(Node * start, Node * end, float cost, Texture* texture);
		void DrawGraph(SpriteBatch* spiteBatch, Font * font);
		//search algorithms
		bool BF_Search(Node* start, Node* end, float deltaTime, bool reset);
		bool DF_Search(Node* start, Node* end, float deltaTime, bool reset);
		bool Dijk_Search(Node* start, Node* end, float deltaTime, bool reset);
		Agent* Dijk_Search(Node* start, std::list<Agent*> *tagetAgents, float deltaTime, float range);
		bool AStar_Search(Node* start, Node* end, float deltaTime, bool reset);
		void clear_Search_Astar();
		void Graph::clear_Search_Dijkstras();
		//helper function
		//bool check_G_ScorePriority(Node* first, Node* second);
	public:
		//Node textures for each condition of search algorithms
		Texture* txtr_redNode;
		Texture* txtr_greenNode;
		Texture* txtr_blueNode;
		Texture* txtr_purpleNode;
		Texture* txtr_edge;
		
	protected: 
		// List of Nodes 

		typedef		std::vector<Node*> NodeVector;
		typedef		std::list<Node*> NodeList;
		NodeVector	nodes;

		bool endFound_Astar;
		bool starter_Astar;
		bool endFound_Dijkstras;
		bool starter_Dijkstras;
		bool altinator_Dijkstras;
		bool altinator_Astar;
	public:
		NodeList	shortestPath;
		vec2		spacing;
		vec2		nodeSize;
		NodeList	nodesList_AStar;
		NodeList	nodesList_Dijkstras;
};