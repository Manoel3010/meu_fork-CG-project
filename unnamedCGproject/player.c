#include <stdio.h>
#include <stdlib.h>

#include <GL/freeglut.h>

#include "player.h"

// Define a macro de implementa��o da biblioteca
#define CGLTF_IMPLEMENTATION
#include "libs/cgltf/cgltf.h"

// Defina a vari�vel global para armazenar modelo do jogadr
// PlayerModel player_model;

void movePlayer(float *speed, Player *playerObject) {
    playerObject->x += speed[0];
    playerObject->y += speed[1];
    playerObject->z += speed[2];
}

// A fun��o agora recebe um ponteiro para a struct Player.
void loadPlayerModel(Player* playerObj, const char* filename) {
    // Inicializa as op��es e tenta carregar o arquicvo GLTF
    cgltf_options options = {0};
    // Tenta carregar o modelo e salva os dados no novo membro 'modelData'.
    cgltf_result result = cgltf_parse_file(&options, filename, &playerObj->modelData);

    if (result != cgltf_result_success) {
        printf("Erro ao carregar o arquivo GLTF: %d\n", result);
        return;
    }

    // Carrega os dados bin�rios do modelo na mem�ria
    result = cgltf_load_buffers(&options, playerObj->modelData, filename);
    if (result != cgltf_result_success) {
        printf("Erro ao carregar os buffers do arquivo GLTF: %d\n", result);
        cgltf_free(playerObj->modelData);
    }
}

//  A fun��o agora recebe um ponteiro para a struct Player.
void drawPlayerModel(Player* playerObj, float thetaAngle) {
    if (!playerObj->modelData) {
        return;
    }

    // A l�gica de desenho do modelo 3D
    // Itera sobre todas as malhas e primitivas do modelo 3D

    glPushMatrix(); // Salva o estado atual da matriz para evitar que o player afete a cena toda
    glTranslatef(playerObj->x, playerObj->y, playerObj->z);
    glRotatef(thetaAngle, 0.0f, 1.0f, 0.0f);
    for (int i = 0; i < playerObj->modelData->meshes_count; ++i) {
        cgltf_mesh* mesh = &playerObj->modelData->meshes[i];

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
    glPopMatrix(); // Restaura o estado da matriz
}

void cleanupPlayerModel(Player* playerObj) {
    // Libera a mem�ria alocada para os dados do modelo 3d
    if (playerObj->modelData) {
        cgltf_free(playerObj->modelData);
        playerObj->modelData = NULL;
    }
}
