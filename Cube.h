#include <ostream>
#include <vector>
#include <cmath>
#include <math.h>
#include <iostream>

using namespace std;

class cube{
    private:

        // A point is what defines a vertex, each point will have a an x, y, and z position
        struct Point{
            Point():x(0), y(0), z(0){}
            Point(double x, double y, double z): x(x), y(y), z(z){}

            double x;
            double y;
            double z;

            friend ostream& operator<<(ostream& os, Point &rhs){
                os << rhs.x << ", " << rhs.y << ", " << rhs.z;
                return os;
            }
        };

        // The edges are what will end up being drawn
        // They are defined by two initial vertices and consist of a vector of points
        // As the shape rotates all of the points will be rotated
        struct Edge{
            double length;
            Point *vertex1;
            Point *vertex2;
            vector<Point*> points;

            Edge() {}

            Edge(Point *p1, Point *p2): Edge(p1, p2, 10) {}

            // Requires the two vertices to be passed in as well as a precision
            // the precision determines how many points will define the line
            Edge(Point *p1, Point *p2, int precision): vertex1(p1), vertex2(p2){
                length = sqrt(pow((p1->x + p2->x), 2) + pow((p1->y + p2->y), 2) + pow((p1->z + p2->z), 2));

                // the dCords are how far away on each axii the next point will be when constructing a line
                double dx = -(p1->x-p2->x)/precision;
                double dy = -(p1->y-p2->y)/precision;
                double dz = -(p1->z-p2->z)/precision;
    
                // Initial positions of the line, the first vertex
                double xPos = p1->x;
                double yPos = p1->y;
                double zPos = p1->z;
                // A point is created at the initial point and then every step up the point untill all the points are created
                points.push_back(new Point(xPos, yPos, zPos));
                for(int i = 0; i<precision; ++i){
                    points.push_back(new Point(xPos+=dx, yPos+=dy, zPos+=dz));
                }
            }

            ostream& printLength(ostream& os){
                for(auto i:points){
                    os << *i << " > ";
                }
                os << endl;
                return os;
            }

            friend ostream& operator<<(ostream& os, Edge &rhs){
                os << *rhs.vertex1 << ": " << *rhs.vertex2;
                return os;
            }
        };

        int sideLength;
        vector<Point*> vertices;
        vector<Edge*> edges;

        void createVertices(){
            // This weird algorithm just creates all the different vertices of a cube based on a determined side length
            // The vertices are appended to a vector of vertices
            for(int i = 0; i < 8; ++i){
                vertices.push_back(new Point());
                vertices[i]->x = (int)((i/4 * -2) + 1) * sideLength*0.5;
                vertices[i]->y = (int)((i%2 * -2) + 1) * sideLength*0.5;
                vertices[i]->z = (int)((i/2%2 * -2) + 1) * sideLength*0.5;
            }
        }

        void generateEdges(int precision){
            //Generates the unique pairs of vertices that need lines drawn through them
            vector<pair<Point, Point>> pointPairs;
            for(int i = 0; i<8; i++){
                for(int j = i; j<8; j++){
                    if(((vertices[i]->x*vertices[j]->x < 0) + (vertices[i]->y*vertices[j]->y < 0) + (vertices[i]->z*vertices[j]->z < 0)) == 1){
                        edges.push_back(new Edge(vertices[i], vertices[j], precision));
                    }
                }
            }
        }

        // takes angle in radians, takes a points cordinates
        void rPitch(double theta, Point *point){
            double x = point->x;
            double z = point->z;

            point->x = x*cos(theta) + z*sin(theta);
            point->z = z*cos(theta) - x*sin(theta);
        }

        void rYaw(double theta, Point* point){
            double x = point->x;
            double y = point->y;

            point->x = x*cos(theta) - y*sin(theta);
            point->y = y*cos(theta) + x*sin(theta);
        }

        void rRoll(double theta, Point* point){
            double y = point->y;
            double z = point->z;

            point->y = y*cos(theta) - z*sin(theta);
            point->z = z*cos(theta) + y*sin(theta);
        }

        void rotateVertices(double yaw, double pitch, double roll){
            yaw *= 2*M_PI/360;
            pitch *= 2*M_PI/360;
            roll *= 2*M_PI/360;

            for(int i = 0; i < 8; ++i){
                Point *point = vertices[i];
                rYaw(yaw, point);
                rPitch(pitch, point);
                rRoll(roll, point);
            }
        }

        void rotateEdges(double yaw, double pitch, double roll){
            yaw *= 2*M_PI/360;
            pitch *= 2*M_PI/360;
            roll *= 2*M_PI/360;

            for(auto edge:edges){
            }
        }

    public:
        cube():cube(10){}

        cube(int length):sideLength(length){
            createVertices();
            generateEdges(10);
        }

        void printVertices(){
            for(int i = 0; i < 8; ++i){
                cout << *vertices[i] << endl;
            }
            cout << endl;
        }

        void printEdges(){
            for(int i = 0; i < edges.size(); ++i){
                cout << *edges[i] << endl;
            }
        }

        void printEdgeLengths(){
            for(int i = 0; i < edges.size(); ++i){
                edges[i]->printLength(cout);
                cout << endl;
            }
        }
// TODO
//FIXME
        friend ostream& operator<<(ostream& os, cube& rhs){
            os << "penis" << endl;
            return os;
        }
};

