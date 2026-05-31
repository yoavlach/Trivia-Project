using System;
using System.Collections.Generic;
using System.Linq;
using System.Linq.Expressions;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace Client
{
    public enum STATISTICS_INDEXES {  AVG_ANSWER_TIME, CORRECT_ANS, TOTAL_ANS, PLAYER_GAMES, HIGH_SCORE };
    /// <summary>
    /// Interaction logic for StatisticsWindow.xaml
    /// </summary>
    public partial class StatisticsWindow : Window
    {
        public StatisticsWindow()
        {
            InitializeComponent();
        }

        private void highScoresButtonPressed(object sender, RoutedEventArgs e)
        {
            HighScoresWindow highScoresWindow = new HighScoresWindow();
            highScoresWindow.Show();
        }

        private void personalStatisticsButtonPressed(object sender, RoutedEventArgs e)
        {
            PersonalStatisticsWindow personalStatisticsWindow = new PersonalStatisticsWindow();
            personalStatisticsWindow.Show();
        }
    }
}
