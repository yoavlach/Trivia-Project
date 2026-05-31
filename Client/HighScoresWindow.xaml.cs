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
    /// Interaction logic for HighScoresWindow.xaml
    /// </summary>
    public partial class HighScoresWindow : Window
    {
        /**
         * Initializes the high scores window and triggers a request to fetch the scores
         */
        public HighScoresWindow()
        {
            InitializeComponent();
            sendHighScoreRequestAndUpdateWindow();
        }

        /**
         * Sends a request to the server to get the top scores and updates the user interface
         * Populates the labels with the top three users and their scores or displays an error message on failure
         */
        private void sendHighScoreRequestAndUpdateWindow()
        {
            byte[] resp;
            HighScoreResponse deserializedResp;
            ErrorResponse errResp;
            resp = Communicator.sendAndReceive(JsonRequestPacketSerializer.serializeRequest(MESSAGE_CODES.HIGH_SCORE));
            try
            {
                switch (Helper.extractMessageCode(resp))
                {
                    case (int)MESSAGE_CODES.HIGH_SCORE:
                        deserializedResp = JsonResponsePacketDeserializer.deserializeHighScoreResponse(resp);
                        switch (deserializedResp.status)
                        {
                            case (int)STATUSES.SUCCESS:
                                firstUsernameLabel.Content = deserializedResp.highScore[0][0];
                                secondUsernameLabel.Content = deserializedResp.highScore[1][0];
                                thirdUsernameLabel.Content = deserializedResp.highScore[2][0];

                                firstUsernameScore.Content = deserializedResp.highScore[0][1];
                                secondUsernameScore.Content = deserializedResp.highScore[1][1];
                                thirdUsernameScore.Content = deserializedResp.highScore[2][1];
                                break;
                            case (int)STATUSES.FAILURE:
                                MessageBox.Show("Unable to retrieve high scores. Please try again");
                                Close();
                                break;
                        }
                        break;

                    case (int)STATUSES.FAILURE:
                        errResp = JsonResponsePacketDeserializer.deserializeErrorResponse(resp);
                        MessageBox.Show(errResp.message);
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