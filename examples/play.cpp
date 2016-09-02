#include <iostream>
#include <pez.h>
#include "glvideo.h"
#include "Movie.h"

using namespace std;

static const std::string VERTEX_SHADER_SOURCE =
        R"EOF(
attribute vec2 Position;
attribute vec2 InCoord;
varying vec2 OutCoord;

void main()
{
    OutCoord = InCoord;
    gl_Position = vec4(Position, 0, 1);
}
)EOF";

static const std::string FRAGMENT_SHADER_SOURCE =
        R"EOF(
varying vec2 OutCoord;
uniform sampler2D Sampler;

void main()
{
   gl_FragColor = texture2D(Sampler, OutCoord);
}
)EOF";


glvideo::Movie::ref movie;

static void BuildGeometry( float aspect );

static void LoadEffect();

enum {
    PositionSlot, TexCoordSlot
};

void PezHandleMouse( int x, int y, int action )
{}

void PezUpdate( unsigned int elapsedMilliseconds )
{}

void PezRender()
{
    glClear( GL_COLOR_BUFFER_BIT );

    glActiveTexture( GL_TEXTURE0 );
    auto frame = movie->getCurrentFrame();
    if ( frame ) {
        glBindTexture( frame->getTextureTarget(), frame->getTextureId());
    }


    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();

    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();

    glDrawArrays( GL_TRIANGLES, 0, 6 );

    glPopMatrix();
    glMatrixMode( GL_PROJECTION );

    glPopMatrix();
    glMatrixMode( GL_MODELVIEW );
}

const char *PezInitialize( int width, int height )
{
    BuildGeometry((float) width / (float) height );
    LoadEffect();

    string filename = "/Users/ian/Desktop/Bed - Side00086523_V1-0001.mov";
//    string filename = "/Users/ian/Desktop/out.mov";

    auto ctx = glvideo::GLContext::makeSharedFromCurrent();
    movie = glvideo::Movie::create( ctx, filename );

    cout << "Format: " << movie->getFormat() << endl;
    cout << "Duration (seconds): " << movie->getDuration() << endl;
    cout << "Size: " << movie->getWidth() << "x" << movie->getHeight() << endl;
    cout << "Framerate: " << movie->getFramerate() << endl;
    cout << "Number of tracks: " << movie->getNumTracks() << endl;
    for ( int i = 0; i < movie->getNumTracks(); ++i ) {
        cout << "\tTrack " << i << " type: " << movie->getTrackDescription( i ) << endl;
    }

    movie->play();

    return "Test Playback";
}


static void BuildGeometry( float aspect )
{
    float X = 1.f;
    float Y = 1.f;
    float verts[] = {
            -X, -Y, 0, 1,
            -X, +Y, 0, 0,
            +X, +Y, 1, 0,
            +X, +Y, 1, 0,
            +X, -Y, 1, 1,
            -X, -Y, 0, 1,
    };

    GLuint vboHandle;
    GLsizeiptr vboSize = sizeof( verts );
    GLsizei stride = 4 * sizeof( float );
    GLenum usage = GL_STATIC_DRAW;
    GLvoid *texCoordOffset = (GLvoid *) (sizeof( float ) * 2);

    glGenBuffers( 1, &vboHandle );
    glBindBuffer( GL_ARRAY_BUFFER, vboHandle );
    glBufferData( GL_ARRAY_BUFFER, vboSize, verts, usage );
    glVertexAttribPointer( PositionSlot, 2, GL_FLOAT, GL_FALSE, stride, 0 );
    glVertexAttribPointer( TexCoordSlot, 2, GL_FLOAT, GL_FALSE, stride, texCoordOffset );
    glEnableVertexAttribArray( PositionSlot );
    glEnableVertexAttribArray( TexCoordSlot );
}

static void LoadEffect()
{
    const char *vsSource = VERTEX_SHADER_SOURCE.c_str(),
            *fsSource = FRAGMENT_SHADER_SOURCE.c_str();
    GLuint vsHandle, fsHandle;
    GLint compileSuccess, linkSuccess;
    GLchar compilerSpew[256];
    GLuint programHandle;

    vsHandle = glCreateShader( GL_VERTEX_SHADER );
    fsHandle = glCreateShader( GL_FRAGMENT_SHADER );

    glShaderSource( vsHandle, 1, &vsSource, 0 );
    glCompileShader( vsHandle );
    glGetShaderiv( vsHandle, GL_COMPILE_STATUS, &compileSuccess );
    glGetShaderInfoLog( vsHandle, sizeof( compilerSpew ), 0, compilerSpew );
    PezCheckCondition( compileSuccess, compilerSpew );

    glShaderSource( fsHandle, 1, &fsSource, 0 );
    glCompileShader( fsHandle );
    glGetShaderiv( fsHandle, GL_COMPILE_STATUS, &compileSuccess );
    glGetShaderInfoLog( fsHandle, sizeof( compilerSpew ), 0, compilerSpew );
    PezCheckCondition( compileSuccess, compilerSpew );

    programHandle = glCreateProgram();
    glAttachShader( programHandle, vsHandle );
    glAttachShader( programHandle, fsHandle );
    glBindAttribLocation( programHandle, PositionSlot, "Position" );
    glBindAttribLocation( programHandle, TexCoordSlot, "InCoord" );
    glLinkProgram( programHandle );
    glGetProgramiv( programHandle, GL_LINK_STATUS, &linkSuccess );
    glGetProgramInfoLog( programHandle, sizeof( compilerSpew ), 0, compilerSpew );
    PezCheckCondition( linkSuccess, compilerSpew );

    glUseProgram( programHandle );

    GLint loc = glGetUniformLocation( programHandle, "Sampler" );
    glUniform1i( loc, 0 );
}
