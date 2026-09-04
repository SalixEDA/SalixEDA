#include <QVector>
#include <QFile>
#include <QRandomGenerator>

class SdMlSoftmaxRegression {
    int             mNumFeatures;
    int             mNumClasses;
    QVector<double> mWeights;     // Матрица K x N
    QVector<double> mBiases;      // Вектор K

  public:
    SdMlSoftmaxRegression(int features, int classes, const double *w ) : mNumFeatures(features), mNumClasses(classes) {
      mWeights.resize( mNumClasses * mNumFeatures );
      mBiases.resize( mNumClasses );
      // Инициализировать веса можно маленькими случайными числами
      if( w == nullptr ) {
        for( int i = 0; i < mNumClasses * mNumFeatures; ++i ) {
          double value = QRandomGenerator::global()->generateDouble();
          mWeights[i] = -0.1 + value * 0.2;
          }

        }
      else {
        int idx = 0;
        for( int i = 0; i < mNumClasses * mNumFeatures; ++i )
          mWeights[i] = w[idx++];
        for( int i = 0; i < mNumClasses; ++i )
          mBiases[i] = w[idx++];
        }
      }



    void store( const QString &fname ) {
      QString str;
      for( int i = 0; i < mNumClasses * mNumFeatures; ) {
        for( int k = 0; k < 16 && i < mNumClasses * mNumFeatures; ++k ) {
          str.append( QString("%1,").arg( mWeights[i++] ) );
          }
        str.append( "\n" );
        }
      for( int i = 0; i < mNumClasses; ++i )
        str.append( QString("%1,").arg( mBiases[i] ) );
      str.append("0.0");
      QFile file(fname);
      if( file.open(QIODevice::WriteOnly) )
        file.write( str.toUtf8() );
      }



    // Прямой проход: получение вероятностей для одного объекта
    QVector<double> predictProba( const QVector<double>& x ) {
      QVector<double> logits( mNumClasses, 0.0 );
      double max_logit = -INFINITY; // Для стабильности Softmax

      for( int c = 0; c < mNumClasses; ++c ) {
        logits[c] = mBiases[c];
        int off = c * mNumFeatures;
        for( int f = 0; f < mNumFeatures; ++f ) {
          logits[c] += mWeights[off + f] * x[f];
          }
        if (logits[c] > max_logit) max_logit = logits[c];
        }

      // Softmax с защитой от переполнения (вычитаем max_logit)
      QVector<double> probs( mNumClasses, 0.0 );
      double sum = 0.0;
      for( int c = 0; c < mNumClasses; ++c ) {
        probs[c] = std::exp(logits[c] - max_logit);
        sum += probs[c];
        }
      for( int c = 0; c < mNumClasses; ++c ) {
        probs[c] /= sum;
        }
      return probs;
      }



    // Один шаг обучения (Stochastic Gradient Descent)
    void trainStep( const QVector<double>& x, int trueClass, double lr ) {
      QVector<double> probs = predictProba(x);

      for( int c = 0; c < mNumClasses; ++c ) {
        // Градиент для Cross-Entropy + Softmax считается очень просто:
        double target = (c == trueClass) ? 1.0 : 0.0;
        double error = probs[c] - target;

        // Обновляем смещения
        mBiases[c] -= lr * error;
        // Обновляем веса
        int off = c * mNumFeatures;
        for( int f = 0; f < mNumFeatures; ++f ) {
          mWeights[off + f] -= lr * error * x[f];
          }
        }
      }
  };

