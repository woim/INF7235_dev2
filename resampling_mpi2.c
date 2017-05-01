#include "resampling_mpi.h"

int _DEBUG_ = 0;

#define DEBUG( x )  if (_DEBUG_ > 0) { x }
#define DEBUG1( x ) if (_DEBUG_ > 1) { x }
#define MSG_STRUCT_IMG 1
#define MSG_DATA_IMG 2
#define MSG_TERMINER 3
#define MSG_REQUETE_TACHE 4
#define MSG_IMG_RES 5
#define MSG_STRUCT_RES 6


void chargerImagesATraiter(Image* source, Image images[])
{
  int nb_rows = source->size[1];
  int origin_offset = 0;
  int offset = 0;
 // printf("creationg de %d images a traiter\n", nb_rows);
  for(int i = 0; i < nb_rows ; ++i)
  {
    Image* image = &(images[i]);
    image->size[0] = source->size[0];
    image->size[1] = 1;
    image->data_size = source->data_size;
    image->origin[0] = source->origin[0];
    image->origin[1] = origin_offset;
    image->length = image->size[0] * image->size[1] * image->data_size;
    image->offset = offset;
    image->data = NULL;
    offset += image->length;
    origin_offset += image->size[1];
  }
}


void coordonnateur( double matrix[2][3], Image* source, Image* destination, int nbTravailleurs )
{
  printf( "coordonnateur myID ( %d )\n", nbTravailleurs ); 

  int nbTravailleursActifs = nbTravailleurs ; // T r a v a i l l e u r s n ’ a y a n t p a s e n c o r e t e r m i n e .
  int nbRows = source->size[1];                        // le nombre de ranges a traiter
  int nbRowsTraitees = 0;                        // le nombre de ranges a traiter
  int i;
  Image* images = malloc(sizeof(Image)*nbRows);
  
  printf("chargement des %d images\n", nbRows);
  chargerImagesATraiter(source, images);
  printf("traitement des images avec %d travailleurs\n", nbTravailleurs);
  
  while ( nbTravailleursActifs > 0 ) 
  {
  
    MPI_Status statut;
    MPI_Recv( NULL, 0, MPI_BYTE, MPI_ANY_SOURCE, MSG_REQUETE_TACHE, MPI_COMM_WORLD, &statut);
    
    printf("Travailleur actif: %d\n",nbTravailleursActifs);
    
    if ( nbRowsTraitees < nbRows-1 ) 
    { 
      printf("Nb rows traitees: %d\n",nbRowsTraitees);
      // Il r e s t e d e s t a c h e s : on la t r a n s m e t .
      MPI_Send(&images[nbRowsTraitees], sizeof(Image), MPI_BYTE, statut.MPI_SOURCE, MSG_STRUCT_IMG, MPI_COMM_WORLD);
      unsigned char* data = malloc(sizeof(unsigned char) * images[nbRowsTraitees].length);
      memcpy(data, source->data + images[nbRowsTraitees].offset, images[nbRowsTraitees].length);      
      MPI_Send(data, images[nbRowsTraitees].length, MPI_UNSIGNED_CHAR, statut.MPI_SOURCE, MSG_DATA_IMG, MPI_COMM_WORLD);

      // Receive the resampled image
      Image img_res;
      MPI_Recv(&img_res, sizeof(Image), MPI_BYTE, MPI_ANY_SOURCE, MSG_STRUCT_RES, MPI_COMM_WORLD, &statut);
      int index = img_res.offset/img_res.length;
      images[index].data = malloc(sizeof(unsigned char) * images[index].length);
      MPI_Recv(images[index].data, images[index].length, MPI_UNSIGNED_CHAR, MPI_ANY_SOURCE, MSG_IMG_RES, MPI_COMM_WORLD, &statut);
      free(data);
      nbRowsTraitees++;
    } 
    else 
    {
      MPI_Send(NULL, 0, MPI_BYTE, nbTravailleursActifs, MSG_TERMINER, MPI_COMM_WORLD);
      nbTravailleursActifs--; // Un de m o i n s e s t m a i n t e n a n t a c t i f .
    }
  }

  for(i = 0 ; i < nbRows ; ++i)
  {
    //memcpy( destination->data+images[i].offset, images[i].data, images[i].length);
    free(images[i].data);
  }
  free(images);  
}

void travailleur(double matrix[2][3] , Image* source, MPI_Comm comm )
{
  int myID;
  MPI_Comm_rank( comm, &myID);

  MPI_Send ( NULL , 0 , MPI_BYTE , 0 , MSG_REQUETE_TACHE , MPI_COMM_WORLD );

  for (;;) 
  {
    Image img;
    MPI_Status statut;

    MPI_Recv(&img, sizeof(Image), MPI_BYTE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &statut);
    if (statut.MPI_TAG == MSG_TERMINER)
    { 
      break;
    }
    
    //On e n v o i e u n e n o u v e l l e r e q u e t e a v a n t de d e b u t e r l ’ a n a l y s e = > c o n c u r r e n c e !
    MPI_Send(NULL, 0, MPI_BYTE, 0, MSG_REQUETE_TACHE, MPI_COMM_WORLD);
   
    img.data = malloc(sizeof(unsigned char) * img.length);
    MPI_Recv(img.data, img.length, MPI_UNSIGNED_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &statut);
  
    resampling(matrix, source, &img);

    MPI_Send(&img, sizeof(Image), MPI_BYTE, 0, MSG_STRUCT_RES, MPI_COMM_WORLD);
    MPI_Send(img.data, img.length, MPI_UNSIGNED_CHAR, 0, MSG_IMG_RES, MPI_COMM_WORLD);
  }
}


////////////////////////////////////////////////////////////////
// Programme principal.
////////////////////////////////////////////////////////////////

void resampling_mpi2(
    double matrix[2][3],
    Image* source,
    Image* destination)
{
  int ID, nbProcs;
  MPI_Comm comm_travailleurs;

  MPI_Comm_rank( MPI_COMM_WORLD, &ID );
  MPI_Comm_size( MPI_COMM_WORLD, &nbProcs );

  // On cree un communicateur independant pour les travailleurs, puis
  // on active le coordonnateur vs. les travailleurs.
  if ( ID == 0 ) {
    // Coordonnateur.
    MPI_Comm_split( MPI_COMM_WORLD, MPI_UNDEFINED, ID, &comm_travailleurs );
    coordonnateur( matrix, source, destination, nbProcs-1 );
  } else {
    // Travailleurs.
    MPI_Comm_split( MPI_COMM_WORLD, 0, ID, &comm_travailleurs );
    travailleur( matrix, source, comm_travailleurs );
  }
}

