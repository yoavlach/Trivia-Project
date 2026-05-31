using System;
using System.Collections.Generic;
using System.Linq;
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
    /// <summary>
    /// Interaction logic for PersonalStatisticsWindow.xaml
    /// </summary>
    public partial class PersonalStatisticsWindow : Window
    {
        public PersonalStatisticsWindow()
        {
            InitializeComponent();
            sendPersonalStatisticsRequestAndUpdateWindow();
        }

        private void sendPersonalStatisticsRequestAndUpdateWindow()
        {
            byte[] resp;
            GetPersonalStatsResponse deserializedResp;
            resp = Communicator.sendAndReceive(JsonRequestPacketSerializer.serializeRequest(MESSAGE_CODES.GET_STATISTICS));
            try
            {
                switch (Helper.extractMessageCode(resp))
                {
                    case (int)MESSAGE_CODES.GET_STATISTICS:
                        deserializedResp = JsonResponsePacketDeserializer.deserializeGetPersonalStatsResponse(resp);
                        switch (deserializedResp.status)
                        {
                            case (int)STATUSES.FAILURE:
                                MessageBox.Show("Unable to retrieve personal statistics. Please try again");
                                Close();
                                break;
                            case (int)STATUSES.SUCCESS:
                                avgAnswerTimeLabel.Content = deserializedResp.statistics[(int)STATISTICS_INDEXES.AVG_ANSWER_TIME];
                                correctAnswersLabel.Content = deserializedResp.statistics[(int)STATISTICS_INDEXES.CORRECT_ANS];
                                totalAnswersLabel.Content = deserializedResp.statistics[(int)STATISTICS_INDEXES.TOTAL_ANS];
                                numGamesLabel.Content = deserializedResp.statistics[(int)STATISTICS_INDEXES.PLAYER_GAMES];
                                highScoreLabel.Content = deserializedResp.statistics[(int)STATISTICS_INDEXES.HIGH_SCORE];
                                break;
                        }
                        break;
                    case (int)STATUSES.FAILURE:
                        MessageBox.Show(JsonResponsePacketDeserializer.deserializeErrorResponse(resp).message);
                        Close();
                        break;
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Unexpected Error: " + ex.Message);
                return;
            }
        }
    }
}
