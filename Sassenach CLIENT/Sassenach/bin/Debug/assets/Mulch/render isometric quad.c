        //Render textured quad
       glBegin( GL_QUADS );
            glTexCoord2f( 0.f, 0.f ); glVertex2f(0.f, mTextureHeight / 2.f);
            glTexCoord2f( 1.f, 0.f ); glVertex2f(mTextureWidth / 2.f, mTextureHeight / 4.f);
            glTexCoord2f( 1.f, 1.f ); glVertex2f(mTextureWidth , mTextureHeight / 2.f);
            glTexCoord2f( 0.f, 1.f ); glVertex2f(mTextureWidth / 2.f, mTextureHeight * 0.75f);
        glEnd();
