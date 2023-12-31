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

            friend Point operator-(Point &lhs, Point &rhs){
                return Point(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
            }

            friend Point operator+(Point &lhs, Point &rhs){
                return Point(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
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

        void rotateEdges(double yaw, double pitch, double roll){
            yaw *= 2*M_PI/360;
            pitch *= 2*M_PI/360;
            roll *= 2*M_PI/360;

            for(auto edge:edges){
                for(auto point:edge->points){
                    rYaw(yaw, point);
                    rPitch(pitch, point);
                    rRoll(roll, point);
                }
            }
        }

        pair<double, double> projectPoint(Point* p){
            //Constants
            Point* camPos = new Point(0, 8, -15);
            double roll = -10;
            double pitch = 0;
            double yaw = 0;
            double viewAngle = 90 /** angle in degrees */ *(2*M_PI/360);

            //This creates a point the has cordinates relative to the position of the camera
            Point* relativePos = new Point((*p) - *camPos);
            rRoll(roll, relativePos);
            rPitch(pitch, relativePos);
            rYaw(yaw, relativePos);

            //Once the point has been shifted to be relative to the camera it can be projected onto the display surface
            double x = relativePos->x/(relativePos->z*tan(viewAngle/2));
            double y = relativePos->y/(relativePos->z*tan(viewAngle/2));
            delete relativePos;
            delete camPos;
            return pair<double, double>(x, y);
        }

    public:
        cube():cube(10){}

        cube(int length):sideLength(length){
            createVertices();
            generateEdges(50);
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

        void printFlatVertices(){
            for(int i = 0; i < 8; ++i){
                auto cord = projectPoint(vertices[i]);
                cout << "x: " << cord.first << ", y: " << cord.second << endl;
            }
            cout << endl;
        }
        
        void printFlatEdges(){
            for(auto edge:edges){
                for(auto point:edge->points){
                    pair<double, double> pos2d = projectPoint(point);
                    cout << pos2d.first << ',' << pos2d.second << " : ";
                }
                cout << endl;
            }
        }

        vector<uint64_t> genereateGraphics(int res){
            vector<uint64_t> cords(64, 0);
            for(auto edge:edges){
                for(auto point:edge->points){
                    pair<double, double> pos2d = projectPoint(point);
                    if(abs(pos2d.first) > 1.0+1e-9 || abs(pos2d.second) > 1.0+1e-9) continue;
                    pos2d.second = pos2d.second>=1.0 ? 63 : (int)((pos2d.second + 1)*32);
                    pos2d.first = pos2d.first>=1.0 ? 63 : (int)((pos2d.first+ 1)*32);
                    cords[pos2d.second] = cords[pos2d.second] | (uint64_t)pow(2, pos2d.first);
                }
            }
            return cords;
        }

        void rotate(double roll, double pitch, double yaw){
            rotateEdges(yaw, pitch, roll);
        }

// TODO
//FIXME
        friend ostream& operator<<(ostream& os, cube& rhs){
            os << "penis" << endl;
            return os;
        }
};

