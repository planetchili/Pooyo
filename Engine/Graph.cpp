
#include "Graph.h"
#include "SpriteBatch.h"
//#include "Texture.h"
//#include "Font.h"
#include <queue>
#include <stack>
#include <list>
//#include "Singleton\Game_Manager.h"
//#include "GameObjects\Agents\Agent.h"

Graph::Node::Node(vec2 pos, Texture* texture)
{
	this->traversed = false;
	this->visited = false;

	this->pos = pos;
	setTexture(texture);

}
Graph::Node::~Node()
{

}
void Graph::Node::setTexture(Texture* texture)
{
	this->texture = texture;
}
Graph::Edge::Edge(Node* start, Node* end, float cost, Texture* texture)
{
	this->Start = start;
	this->End = end;
	this->cost = cost;

	this->texture = texture;
	vec2 origVector = this->End->pos - this->Start->pos;
	this->centre = this->End->pos - (origVector / 2.0f);
	this->length = glm::length(origVector);
	if (origVector != vec2(0))
	{
		vec2 dir = glm::normalize(origVector);
		this->rotation = atan2(dir.y, dir.x);
	}
	else
	{
		this->rotation = 0.0f;
	}
}
Graph::Graph()
{
	LoadTextures();
	this->altinator_Astar = false;
	this->altinator_Dijkstras = false;
}
void Graph::LoadTextures()
{
	this->txtr_blueNode		= Game_Manager::getInst()->txtr_node_water_transp;
	this->txtr_redNode		= Game_Manager::getInst()->txtr_node_Red_transp;
	this->txtr_greenNode	= Game_Manager::getInst()->txtr_node_Green_transp;
	this->txtr_purpleNode	= Game_Manager::getInst()->txtr_node_Purple_transp;
	this->txtr_edge			= Game_Manager::getInst()->txtr_edge_transp;
}
void Graph::MakeGrid(vec2 size, vec2 spacing,vec2 startPos)
{
	
	this->spacing = spacing;
	this->nodeSize = size;
	vec2 pos(0.0f, 0.0f);
	auto itr = nodes.begin();
	for (int i = 0; i < size.y; i++)
	{
		pos.y = i * spacing.y;

		for (int j = 0; j< size.x; j++)
		{
			pos.x = j * spacing.x;
			AddNode(pos + spacing / 2.0f, this->txtr_blueNode);
		}
	}
	//for (int i = 0; i < this->nodes.size(); i++)
	int i = 0;
	for (auto nodePtr : nodes)
	{

		//itr = *nodes[i];
		//right
		if (i < this->nodes.size() - size.x)
			ConnectNodes(nodePtr, nodes[i + (int)size.x], 1, this->txtr_edge);
		//left
		if (i >= size.x)
			ConnectNodes(nodePtr, nodes[i - (int)size.x], 1, this->txtr_edge);
		//down
		if ((i + 1) % (int)(size.x))
			ConnectNodes(nodePtr, nodes[i + 1], 1, this->txtr_edge);
		//up
		if (i > 0 && i % (int)size.x)
			ConnectNodes(nodePtr, nodes[i - 1], 1, this->txtr_edge);
		//diag right and down
		if ((i + 1) < (int)this->nodes.size() - (int)size.x && (i + 1) % (int)(size.x))
			ConnectNodes(nodePtr, nodes[i + 1 + (int)size.x], 1, this->txtr_edge);
		//diag left and down
		if ((i - 1) < (int)this->nodes.size() - (int)size.x)
			ConnectNodes(nodePtr, nodes[i - 1 + (int)size.x], 1, this->txtr_edge);

		i++;
	}
	clear_Search_Astar();
	clear_Search_Dijkstras();
};
Graph::~Graph()
{

}
Graph::Node* Graph::AddNode(vec2 pos, Texture* texture)
{
	if (FindNode(pos) == NULL)
	{
		Node* newNode = new Node(pos, texture);
		nodes.push_back(newNode);
		return newNode;
	}
	else
	{
		return NULL;
	}
}
Graph::Node* Graph::FindNode(vec2 pos) // find node with value 
{
	auto it = this->nodes.begin();;
	bool nodeFound = false;
	while (it != this->nodes.end() && !nodeFound)
	{
		if ((*it)->pos == pos)
			nodeFound = true;
		else
			it++;
	}
	if (it != this->nodes.end())
	{
		return *it;
	}
	else
		return NULL;
}
Graph::Node* Graph::FindNode(vec2 pos, float range) // find node with value 
{
	auto it = this->nodes.begin();;
	bool nodeFound = false;
	while (it != this->nodes.end() && !nodeFound)
	{
		vec2 diff = (*it)->pos - pos;
		float len = glm::length(diff);
		//if ((*it)->pos == pos)
		if (len <= range)
			nodeFound = true;
		else
			it++;
	}
	if (it != this->nodes.end())
	{
		return *it;
	}
	else
		return NULL;
}
void Graph::RemoveConnectingEdges(Node *pNode)
{
	for (auto edgePtr : pNode->edges)
	{
		bool endNodeEdgeFound = false;
		auto it_endNodeEdges = edgePtr->End->edges.begin();
		while (!endNodeEdgeFound && it_endNodeEdges != edgePtr->End->edges.end())
		{
			if ((*it_endNodeEdges)->End == pNode)
				endNodeEdgeFound = true;
			else
				it_endNodeEdges++;
		}
		if (it_endNodeEdges != edgePtr->End->edges.end())
		{
			edgePtr->End->edges.erase(it_endNodeEdges);
			edgePtr->End->edges.shrink_to_fit();
		}
	}

	pNode->edges.clear();
	pNode->edges.shrink_to_fit();

}
void Graph::RemoveNode(Node *pNode) // remove the given node 
{
	if (pNode != NULL)
	{
		auto itNode = nodes.begin();
		bool nodeFound = false;
		RemoveConnectingEdges(pNode);

		while (itNode != nodes.end() && !nodeFound)
		{
			if (*itNode == pNode)
				nodeFound = true;
			else
				itNode++;
		}
		if (itNode < nodes.end())
		{
			delete *itNode;
			nodes.erase(itNode);
		}
	}
}
void Graph::ConnectNodesInRange(Node *pNode, float range, Texture* texture)
{
	
	for (auto nodePtr : nodes)
	{
		vec2 diff = nodePtr->pos - pNode->pos;
		float len = glm::length(diff);
		if (glm::length(nodePtr->pos - pNode->pos) <= range && nodePtr != pNode)
		{
			ConnectNodes(pNode, nodePtr, 1.0f, texture);
		}
	}
}
void Graph::ConnectNodes(Node *start, Node *end, float cost, Texture* texture)
{
	Edge* newEdge = new Edge(start, end, cost, texture);
	start->edges.push_back(newEdge);
	newEdge = new Edge(end, start, cost, texture);
	end->edges.push_back(newEdge);
}
void Graph::DrawGraph(SpriteBatch* spriteBatch, Font *font)
{
	
	for (auto nodePtr : nodes)
	{
		for (auto edgePtr : nodePtr->edges)
		{
			spriteBatch->DrawSprite(edgePtr->texture, edgePtr->centre.x, edgePtr->centre.y, edgePtr->length, 2.0f, edgePtr->rotation);
		}
	}
	for (auto nodePtr : nodes)
	{
		spriteBatch->DrawSprite(nodePtr->texture, nodePtr->pos.x, nodePtr->pos.y, this->spacing.x, this->spacing.y);
	}
	if (!shortestPath.empty())
	{
		int nodeNum = 0;
		char charNum[20] = { NULL };
		for (auto nodePtr : shortestPath)
		{
			
			_itoa_s(nodeNum, charNum, 10);
			float lengthX = ((float)strlen(charNum)*11.0f * 0.25f);
			float lengthY = 11.0f * 0.5f;
			
			spriteBatch->DrawString(font, charNum, nodePtr->pos.x - lengthX, nodePtr->pos.y - lengthY);
			
			nodeNum++;
		}
		
	}
}
bool Graph::BF_Search(Node* start, Node* end, float deltaTime, bool reset)
{
	static bool starter = false;
	static bool endFound = false;
	static std::queue<Node*> oNodeQueue;
	if (!starter)
	{

		reset = true;
		starter = true;

	}
	if (reset)
	{
		shortestPath.clear();
		endFound = false;
		while (!oNodeQueue.empty())
		{
			oNodeQueue.pop();
		}
		for (auto nodesPtr : nodes)
		{
			nodesPtr->traversed = false;
			nodesPtr->visited = false;
			nodesPtr->texture = this->txtr_blueNode;
			nodesPtr->previousNode = NULL;
		}
		if (start != NULL)
		{
			oNodeQueue.push(start);
			starter = true;
		}
		if (end != NULL)
			end->setTexture(this->txtr_purpleNode);
	}
	
	
	float dt = deltaTime;
	static float accumT = 0.0f;
	static float timer = 0.0f;
	accumT += dt;
	while (!oNodeQueue.empty() && !endFound && accumT > timer)
	{
		accumT = 0.0f;
		Node* pCurrent = oNodeQueue.front();
		oNodeQueue.pop();
		if (pCurrent->traversed == true)
		{
			continue;
		}
		pCurrent->traversed = true;
		pCurrent->visited = true;
		pCurrent->setTexture(this->txtr_redNode);
		if (pCurrent == end)
		{
			endFound = true;
			Node* cur = end;
			cur->setTexture(this->txtr_purpleNode);
			while (cur->previousNode != cur && cur->previousNode != NULL)
			{
				cur->previousNode->setTexture(this->txtr_purpleNode);
				shortestPath.push_front(cur);
				cur = cur->previousNode;
			}
			shortestPath.push_front(start);
			return true;
		}
		for (auto edgesPtr : pCurrent->edges)
		{
			if (!edgesPtr->End->traversed)
				edgesPtr->End->previousNode = edgesPtr->Start;
			if (edgesPtr->End->visited != true)
			{
				oNodeQueue.push(edgesPtr->End);
				edgesPtr->End->visited = true;
				edgesPtr->End->setTexture(this->txtr_greenNode);
			}
		}
	}
	return false;
}
bool Graph::DF_Search(Node* start, Node* end, float deltaTime, bool reset)
{
	static bool starter = false;
	static bool endFound = false;
	static std::stack<Node*> oNodeStack;
	if (!starter)
	{
		starter = true;
		reset = true;
	}
	if (reset)
	{
		shortestPath.clear();
		endFound = false;
		while (!oNodeStack.empty())
		{
			oNodeStack.pop();
		}
		for (auto nodesPtr : nodes)
		{
			nodesPtr->traversed = false;
			nodesPtr->visited = false;
			nodesPtr->texture = this->txtr_blueNode;
			nodesPtr->previousNode = NULL;
		}
		if (start != NULL)
		{
			oNodeStack.push(start);
			starter = true;
		}
		if (end != NULL)
			end->setTexture(this->txtr_purpleNode);
	}
	
	float dt = deltaTime;
	static float accumT = 0.0f;
	static float timer = 0.0f;

	//keep looping until the stack is empty. 
	//This will only happen once we've checked every node.
	accumT += dt;

	while (!oNodeStack.empty() && !endFound && accumT > timer)
	{
		accumT = 0.0f;
		Node* pCurrent = oNodeStack.top();
		oNodeStack.pop();

		if (pCurrent->traversed == true)
		{
			continue;
		}
		pCurrent->traversed = true;
		pCurrent->visited = true;
		pCurrent->setTexture(this->txtr_redNode);
		if (pCurrent == end)
		{
			endFound = true;
			Node* cur = end;
			cur->setTexture(this->txtr_purpleNode);
			while (cur->previousNode != cur && cur->previousNode != NULL)
			{

				cur->previousNode->setTexture(this->txtr_purpleNode);
				shortestPath.push_front(cur);
				cur = cur->previousNode;
			}
			shortestPath.push_front(start);
			return true;
		}
		
		for (auto edgesPtr : pCurrent->edges)
		{
			if (!edgesPtr->End->traversed)
				edgesPtr->End->previousNode = edgesPtr->Start;
			if (edgesPtr->End->visited != true)
			{
				oNodeStack.push(edgesPtr->End);
				edgesPtr->End->visited = true;
				edgesPtr->End->setTexture(this->txtr_greenNode);
			}
		}
	}
	return false;
}
bool check_G_ScorePriority(Graph::Node* first, Graph::Node* second)
{
	return (first->G_Score < second->G_Score);
}
bool Graph::Dijk_Search(Node* start, Node* end, float deltaTime, bool reset)
{
	static std::list<Node*> oNodeList;
	static float MAX_GSCORE = FLT_MAX;
	static bool starter = false;
	static bool endFound = false;
	if (!starter)
	{
		reset = true;
		starter = true;
	}
	if (reset)
	{
		endFound = false;
		oNodeList.clear();
		shortestPath.clear();
		//reset all nodes
		for (std::vector<Node*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
		{
			Node* current = *it;

			current->traversed = false;
			current->visited = false;
			current->G_Score = MAX_GSCORE;
			current->previousNode = NULL;
			current->setTexture(txtr_blueNode);
		}
		if (start != NULL)
		{
			//put start node in priority queue (but am using a list to mimc a priority queue)
			oNodeList.push_back(start);
			//set starting nodes previous node to its self
			start->previousNode = start;
			//set starting nodes GScore to equal ZERO
			start->G_Score = 0;
		}
		if (end != NULL)
			end->setTexture(this->txtr_purpleNode);
	}
	float dt = deltaTime;
	static float accumT = 0.0f;
	static float timer = 0.00f;
	accumT += dt;

	//start Dijkstra's search from starting node
	while (!oNodeList.empty() && !endFound && accumT > timer)
	{
		accumT = 0.0f;
		//get and remove first priority from queue
		Node* pCurrent = oNodeList.front();
		oNodeList.pop_front();
		if (pCurrent->traversed == true)
		{
			continue;
		}
		//marke as traversed and visited
		pCurrent->traversed = true;
		pCurrent->visited = true;
		pCurrent->setTexture(txtr_redNode);//render purposes only: RED to show node has been traversed
		if (pCurrent == end)
		{
			endFound = true;
			Node* cur = end;
			cur->setTexture(this->txtr_purpleNode);
			while (cur->previousNode != cur && cur->previousNode != NULL)
			{

				cur->previousNode->setTexture(this->txtr_purpleNode);
				shortestPath.push_front(cur);
				cur = cur->previousNode;
			}
			shortestPath.push_front(start);
			return true;
		}
		//loop through current nodes edges
		for (auto edgePtr : pCurrent->edges)
		{
			//check that node has not been marked as traversed
			if (!edgePtr->End->traversed)
			{
				//calculate current GSccore + edge cost
				float Cur_G_Score = edgePtr->Start->G_Score + edgePtr->cost;
				//validate current GScore cost with connecting nodes cost
				if (Cur_G_Score < edgePtr->End->G_Score)
				{
					//update Dijkstra's shortest path details (incrementaly of course)
					edgePtr->End->previousNode = edgePtr->Start;
					//update the connecting nodes GScore value as is currently least GScore - cost
					edgePtr->End->G_Score = Cur_G_Score;

					//given that we know the current GScore has a lesser cost
					//we check that the conencting node is not already in the priority queue
					if (edgePtr->End->visited != true)
					{
						//then we push the connecting node in the priority queue
						oNodeList.push_back(edgePtr->End);
						//we now set the connecting node as being in the priority queue
						edgePtr->End->visited = true;
						//mimic the list as a priority queue by sorting it based on the least GScore currently in list
						oNodeList.sort(&check_G_ScorePriority);
						edgePtr->End->setTexture(this->txtr_greenNode);//render purposes only: GREEN to show node is in the priority queue

					}
				}
			}
		}
	}
	if (oNodeList.empty())
	{
		return true;
	}
	else
	{
		return false;
	}
}
Agent* Graph::Dijk_Search(Node* start, std::list<Agent*> *tagetAgents, float deltaTime, float range)
{
	Node * closestTargetNode = NULL;
	Agent * closestTargetAgent = NULL;

	//static std::list<Node*> oNodeList;
	static int MAX_GSCORE = INT_MAX;
	//static bool starter = false;
	//static bool endFound = false;
	if (!starter_Dijkstras)
	{
		clear_Search_Dijkstras();
		starter_Dijkstras = true;
		
	}

	if (start != NULL)
	{
		//put start node in priority queue (but am using a list to mimc a priority queue)
		nodesList_Dijkstras.push_back(start);
		//set starting nodes previous node to its self
		start->previousNode = start;
		//set starting nodes GScore to equal ZERO
		start->G_Score = 0;
	}
	float dt = deltaTime;
	static float accumT = 0.0f;
	static float timer = 0.00f;
	accumT += dt;

	altinator_Dijkstras = !altinator_Dijkstras;
	
	//start Dijkstra's search from starting node
	while (!nodesList_Dijkstras.empty() && !endFound_Dijkstras && altinator_Dijkstras == true /*accumT > timer*/)
	{
		altinator_Dijkstras = !altinator_Dijkstras;
		accumT = 0.0f;
		//mimic the list as a priority queue by sorting it based on the least GScore currently in list
		//nodesList_Dijkstras.sort(&check_G_ScorePriority);
		//get and remove first priority from queue
		Node* pCurrent = nodesList_Dijkstras.front();
		nodesList_Dijkstras.pop_front();
		if (pCurrent->traversed == true)
		{
			continue;
		}
		//marke as traversed and visited
		pCurrent->traversed = true;
		pCurrent->visited = true;
		pCurrent->setTexture(txtr_redNode);//render purposes only: RED to show node has been traversed
		
		for (auto targetPtr : *tagetAgents)
		{
			closestTargetNode = FindNode(targetPtr->ballCollider->position, length(this->spacing) *0.5f);
			if (pCurrent == closestTargetNode)
			{
				endFound_Dijkstras = true;
				Node* cur = closestTargetNode;
				cur->setTexture(this->txtr_purpleNode);
				while (cur->previousNode != cur && cur->previousNode != NULL)
				{

					cur->previousNode->setTexture(this->txtr_purpleNode);
					//shortestPath.push_front(cur);
					cur = cur->previousNode;
				}
				//shortestPath.push_front(start);
				/////AGENT FOUND!!! .....  ATTACK!!!
				closestTargetAgent =  targetPtr;
			}
		}
		
		//loop through current nodes edges
		for (auto edgePtr : pCurrent->edges)
		{
			//check that node has not been marked as traversed
			if (!edgePtr->End->traversed)
			{
				//calculate current GSccore + edge cost
				float Cur_G_Score = edgePtr->Start->G_Score + edgePtr->cost;
				//validate current GScore cost with connecting nodes cost
				if (Cur_G_Score < edgePtr->End->G_Score)
				{
					//update Dijkstra's shortest path details (incrementaly of course)
					edgePtr->End->previousNode = edgePtr->Start;
					//update the connecting nodes GScore value as is currently least GScore - cost
					edgePtr->End->G_Score = Cur_G_Score;

					//given that we know the current GScore has a lesser cost
					//we check that the conencting node is not already in the priority queue
					if (edgePtr->End->visited != true)
					{
						//then we push the connecting node in the priority queue
						nodesList_Dijkstras.push_back(edgePtr->End);
						//we now set the connecting node as being in the priority queue
						edgePtr->End->visited = true;

						edgePtr->End->setTexture(this->txtr_greenNode);//render purposes only: GREEN to show node is in the priority queue

					}
				}
			}
		}
		if (length(pCurrent->pos - start->pos) >= range)
		{
			clear_Search_Dijkstras();
		}
	}
	
	return closestTargetAgent;
}
bool check_F_ScorePriority(Graph::Node* first, Graph::Node* second)
{
	return(first->F_Score < second->F_Score);
}
bool Graph::AStar_Search(Node* start, Node* end, float deltaTime, bool reset)
{
	static float MAX_GSCORE = FLT_MAX;

	if (!starter_Astar)
	{
		clear_Search_Astar();
		reset = true;
		starter_Astar = true;
		
		// start node in priority queue (but am using a list to mimc a priority queue)
		nodesList_AStar.push_back(start);
		//set starting Node previous node to its self
		start->previousNode = start;
		//set starting Node GScore to equal ZERO
		start->G_Score = 0;
	}

	altinator_Astar = !altinator_Astar;
	while (!nodesList_AStar.empty() && !endFound_Astar && altinator_Astar == true)
	{
		altinator_Astar = !altinator_Astar;
		//get and remove first priority from queue
		Node* pCurrent = nodesList_AStar.front();
		nodesList_AStar.pop_front();
		if (pCurrent->traversed == true)
		{

			continue;
		}
		//marke as traversed and visited
		pCurrent->traversed = true;
		pCurrent->visited = true;
		pCurrent->setTexture(this->txtr_redNode);
		if (pCurrent == end)
		{
			endFound_Astar = true;
			Node* currentNode = end;
			shortestPath.clear();
			while (currentNode->previousNode != currentNode && currentNode->previousNode != NULL)
			{
				currentNode->setTexture(this->txtr_purpleNode);
				shortestPath.push_front(currentNode);
				currentNode = currentNode->previousNode;

			}
			start->setTexture(this->txtr_purpleNode);
			shortestPath.push_front(start);
			return true;
		}
		//loop through current Node edges
		for (auto it = pCurrent->edges.begin(); it != pCurrent->edges.end(); ++it)
		{
			Edge* current = *it;
			//check that node has not been marked as traversed
			if (!current->End->traversed)
			{
				//calculate current GSccore + edge cost
				float cur_GScore = current->Start->G_Score + current->cost;
				//validate current GScore cost with connecting Node cost
				if (cur_GScore < current->End->G_Score)
				{
					//update the connecting Node GScore value as is currently least GScore cost
					current->End->G_Score = cur_GScore;
				}
				//current->End->G_Score = cur_GScore;//TEST2
				float cur_FScore = current->Start->G_Score + (current->cost + glm::length(end->pos - current->End->pos));
				if (cur_FScore < current->End->F_Score/* && current->End->traversable*/)
				{
					//update the connecting Node GScore value as is currently least GScore cost
					//current->End->G_Score = cur_GScore;//TEST1

					//update the connecting Node FScore value as is currently least FScore cost
					current->End->F_Score = cur_FScore;

					//update ASTAR's shortest path details (incrementaly of course)
					current->End->previousNode = current->Start;

					//given that we know the current GScore has a lesser cost
					//we check that the conencting node is not already in the priority queue
					if (current->End->visited != true)
					{
						//then we push the connecting node in the priority queue
						nodesList_AStar.push_back(current->End);
						//we now set the connecting node as being in the priority queue
						current->End->visited = true;
						//change nodes texture colour to green for visual display showing node as visited
						current->End->setTexture(this->txtr_greenNode);
						//mimic the list as a priority queue by sorting it based on the least GScore currently in list
						nodesList_AStar.sort(check_F_ScorePriority);
					}
				}
			}
		}
	}
	if (endFound_Astar || nodesList_AStar.empty())
	{
		clear_Search_Astar();
	}

	return false;
}
void Graph::clear_Search_Astar()
{
		static float MAX_GSCORE = FLT_MAX;
		starter_Astar = false;
		endFound_Astar = false;

		//reset all Node
		for (std::vector<Node*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
		{
			Node* current = *it;
			current->setTexture(this->txtr_blueNode);
			current->traversed = false;
			current->visited = false;
			current->G_Score = MAX_GSCORE;
			current->F_Score = MAX_GSCORE;
			current->previousNode = NULL;

		}
		nodesList_AStar.clear();
		//shortestPath.clear();
}
void Graph::clear_Search_Dijkstras()
{
	static float MAX_GSCORE = FLT_MAX;
	starter_Dijkstras = false;
	endFound_Dijkstras = false;
	//shortestPath.clear();

	//reset all Node
	for (std::vector<Node*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
	{
		Node* current = *it;
		current->setTexture(this->txtr_blueNode);
		current->traversed = false;
		current->visited = false;
		current->G_Score = MAX_GSCORE;
		current->F_Score = MAX_GSCORE;
		current->previousNode = NULL;
	}
	nodesList_Dijkstras.clear();
}