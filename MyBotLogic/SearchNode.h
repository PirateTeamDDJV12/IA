#ifndef SEARCHNODE_HEADER
#define SEARCHNODE_HEADER

class SearchNode
{
    SearchNode* m_parent;
    
    unsigned int m_xpos; //x position (column number)
    unsigned int m_ypos; //y position (row number)

    unsigned int m_id;  // node Id
    unsigned int m_H;   // heuristic
    unsigned int m_G;   // total cost

public:
    SearchNode(unsigned xpos, unsigned ypos, unsigned id, SearchNode* search_node)
        : m_xpos(xpos), m_ypos(ypos), m_id(id),
        m_parent(search_node), m_H(0), m_G(0)
    {}

    SearchNode(unsigned xpos, unsigned ypos, unsigned id)
        : m_xpos(xpos), m_ypos(ypos), m_id(id),
        m_parent(nullptr), m_H(0), m_G(0)
    {}


    /*Get the x node position*/
    unsigned int getX() const noexcept;

    /*Get the y node position*/
    unsigned int getY() const noexcept;

    /*Get the Node Id*/
    unsigned int getId() const noexcept;

    /*get the heuristic of the node*/
    unsigned int getH() const noexcept;

    /*get the total cost the node*/
    unsigned int getG() const noexcept;

    /*get the cost and the heuristic of the node*/
    unsigned int getF() const noexcept;

    /*get the parent of the node*/
    SearchNode* getParent() const noexcept;


    void setX(unsigned int x) noexcept;
    void setY(unsigned int y) noexcept;
    void setH(const  unsigned int H) noexcept;
    void setG(const  unsigned int G) noexcept;
    void setParent(SearchNode* parent) noexcept;
};

#endif // SEARCHNODE_HEADER