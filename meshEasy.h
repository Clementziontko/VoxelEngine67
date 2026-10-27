#pragma once
#include <glad/glad.h>
#include <vector>
#include <cstddef> 

class MeshEasy {
public:
    unsigned int VAO;
    
    MeshEasy(){}
 
    MeshEasy(const std::vector<float>& vertices_c, const std::vector<unsigned int>& indices_c, bool czy_indices_c = false, bool AttributesPlus_c = false)
    :vertices(vertices_c),
    indices(indices_c), czy_indices(czy_indices_c),
    AttributesPlus(AttributesPlus_c){
        setUp();
    }

    MeshEasy(const std::vector<float>& vertices_c, bool czy_indices_c = false, bool AttributesPlus_c = false) : vertices(vertices_c), czy_indices(czy_indices_c), AttributesPlus(AttributesPlus_c){
        setUp();
    }




    ~MeshEasy() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
      
    }

    // Funkcja do rysowania kształtu
    void Draw() const {
        glBindVertexArray(VAO);
        // Dzielimy rozmiar przez 3, bo każdy punkt ma 3 współrzędne (X, Y, Z)
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }

    void DrawIndices() const{
        glBindVertexArray(VAO);
        // Dzielimy rozmiar przez 3, bo każdy punkt ma 3 współrzędne (X, Y, Z)
        // glDrawElements(GL_LINES, 0, vertices.size() / 3, indices.data());
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
      
        glBindVertexArray(0);
    }

    void DrawLines() const {
        glBindVertexArray(VAO);
        // Dzielimy rozmiar przez 3, bo każdy punkt ma 3 współrzędne (X, Y, Z)
        // glDrawElements(GL_LINES, 0, vertices.size() / 3, indices.data());
        glDrawElements(GL_LINES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
      
        glBindVertexArray(0);
    }

    void setVertices(std::vector<float>& vertices_f){
        vertices = vertices_f;
    }

    void setIndices(std::vector<unsigned int>& indices_f){
        indices = indices_f;
    }

    void setCzyIndices(bool czy_indices_f){
        czy_indices = czy_indices_f;
    }

    void wywolajsetUp(){
        setUp();
    }

private:
    bool czy_indices, AttributesPlus;
    unsigned int VBO, EBO;
    std::vector<float> vertices; 
    std::vector<unsigned int> indices;

    void setUp() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        if(czy_indices){
        glGenBuffers(1, &EBO);
        }
        glBindVertexArray(VAO);

        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);  

        if(czy_indices){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);   
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        // if(AttributesPlus){

        // }

        }




        if(!czy_indices){
        glEnableVertexAttribArray(0);   
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

        glEnableVertexAttribArray(1);   
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

        glEnableVertexAttribArray(2);   
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        }
        
        glBindVertexArray(0);
    }
};