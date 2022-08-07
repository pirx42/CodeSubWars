// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "Constants.h"
#include "AboutDialog.h"
#include "CSWLogo.h"


namespace CodeSubWars
{

  AboutDialog::AboutDialog(QWidget* pParent)
  : QDialog(pParent, Qt::WindowTitleHint | Qt::MSWindowsFixedSizeDialogHint)
  {
    setWindowTitle("About");
    setSizeGripEnabled(false);

    QVBoxLayout* pMainLayout = new QVBoxLayout(this);
    pMainLayout->setSpacing(15);

      QHBoxLayout* pInformationLayout = new QHBoxLayout;
      pInformationLayout->setSpacing(15);
      pMainLayout->addLayout(pInformationLayout);

        QByteArray data(reinterpret_cast<const char*>(LOGO), sizeof(LOGO));
        QPixmap pxmp;
        pxmp.loadFromData(data);
        QLabel* pPicture = new QLabel(this);
        pPicture->setFrameStyle(QFrame::Box | QFrame::Plain);
        pPicture->setLineWidth(1);
        pPicture->setPixmap(pxmp);
        pPicture->setFixedSize(pxmp.size());
        pInformationLayout->addWidget(pPicture);

        QVBoxLayout* pDetailsLayout = new QVBoxLayout;
        pInformationLayout->addLayout(pDetailsLayout);
      
          QLabel* pVersion = new QLabel(QString("CodeSubWars") + " v" + Constants::getVersion().getAsString().c_str() + " beta\n"
                                        "Copyright (c) 2005-2022 " + Constants::AUTHOR + ". All rights reserved.\n" +
                                        "Released under the MIT license. (see license.txt)", this);
          pVersion->setAlignment(Qt::AlignLeft | Qt::AlignTop);
          pDetailsLayout->addWidget(pVersion);

          QLabel* pText = new QLabel(QString() +
            "A physics based three dimensional programming game.\n" +
            "\n" +
            Constants::WEBSITE + "\n" +
            Constants::EMAIL + "\n" +
            "\n" +
            "based on (alphabetical order):\n" + 
            "    Boost 1.79.0 for python bindings and transforms (http://www.boost.org/)\n" +
            "    GLUT 3.7 for 3d visualization (https://www.opengl.org/)\n" +
            "    OpenGL for 3d visualization (http://www.opengl.org/)\n" +
            "    Python 3.10.5 for interpreting python code (http://www.python.org/)\n" +
            "    Qt 5.12.11 for gui (http://www.qt.io/)\n" +
            "    SOLID 3.5.4 for collision detection (http://www.dtecta.com/)"
            , this);
          pText->setAlignment(Qt::AlignLeft);
          pDetailsLayout->addWidget(pText);

      QHBoxLayout* pButtonLayout = new QHBoxLayout;
      pMainLayout->addLayout(pButtonLayout);
    
        pButtonLayout->addStretch();

        QPushButton* pButton = new QPushButton(tr("Ok"), this);
        pButton->setFocus();
        pButtonLayout->addWidget(pButton);
        connect(pButton, SIGNAL(clicked()), this, SLOT(accept()));      

        pButtonLayout->addStretch();
  }


  AboutDialog::~AboutDialog()
  {
  }
}

#include "AboutDialogMoc.cpp"
