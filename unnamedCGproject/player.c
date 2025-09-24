#include <stdio.h>
#include <stdlib.h>

#include <GL/freeglut.h>

#include "player.h"

// Define a macro de implementa��o da biblioteca
#define CGLTF_IMPLEMENTATION
#include "libs/cgltf/cgltf.h"

// Defina a vari�vel global para armazenar modelo do jogadr
PlayerModel player_model;

void movePlayer(float *speed, Player *playerObject) {
    playerObject->x += speed[0];
    playerObject->y += speed[1];
    playerObject->z += speed[2];
}

void loadPlayerModel(const char* filename) {
    // Inicializa as op��es e tenta carregar o arquicvo GLTF
    cgltf_options options = {0};
    cgltf_result result = cgltf_parse_file(&options, filename, &player_model.data);

    if (result != cgltf_result_success) {
        printf("Erro ao carregar o arquivo GLTF: %d\n", result);
        return;
    }

    // Carrega os dados bin�rios do modelo na mem�ria
    result = cgltf_load_buffers(&options, player_model.data, filename);
    if (result != cgltf_result_success) {
        printf("Erro ao carregar os buffers do arquivo GLTF: %d\n", result);
        cgltf_free(player_model.data);
    }
}

void drawPlayerModel(Player playerObj) {
    if (!player_model.data) {
        return;
    }

    // A l�gica de desenho do modelo 3D
    // Itera sobre todas as malhas e primitivas do modelo 3D

    glTranslatef(playerObj.x, playerObj.y, playerObj.z);
    for (int i = 0; i < player_model.data->meshes_count; ++i) {
        cgltf_mesh* mesh = &player_model.data->meshes[i];

        for (int j = 0; j < mesh->primitives_count; ++j) {
            cgltf_primitive* primitive = &mesh->primitives[j];

            if (primitive->type == cgltf_primitive_type_triangles) {
                cgltf_accessor* positions_accessor = primitive->attributes[0].data;
                cgltf_accessor* normals_accessor = primitive->attributes[1].data;
                cgltf_accessor* indices_accessor = primitive->indices;


                glBegin(GL_TRIANGLES);

                // Itera sobre os �nddices para desenhar os tri�ngulos dos modelos
                for (cgltf_size k = 0; k < indices_accessor->count; ++k) {
                    // L� o �ndice do v�rtice atual
                    cgltf_size index = cgltf_accessor_read_index(indices_accessor, k);

                    // L� a posi��o do v�rtice e desenha com a fun��o OpenGl
                    float position[3];
                    cgltf_accessor_read_float(positions_accessor, index, position, 3);
                    glVertex3f(position[0], position[1], position[2]);

                    // L� a normal do v�rtice e a define para o c�lculo de ilumina��o
                    float normal[3];
                    cgltf_accessor_read_float(normals_accessor, index, normal, 3);
                    glNormal3f(normal[0], normal[1], normal[2]);
                }
                glEnd();
            }
        }
    }
}

void cleanupPlayerModel() {
    // Libera a mem�ria alocada para os dados do modelo 3d
    if (player_model.data) {
        cgltf_free(player_model.data);
        player_model.data = NULL;
    }
}
