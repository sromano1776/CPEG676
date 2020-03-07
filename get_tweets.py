import tweepy
import sys

def access_Twitter(handle):
        consumer_key=""
        consumer_secret=""
        access_key=""
        access_secret=""

        api=tweepy.OAuthHandler(consumer_key, consumer_secret)
        api.set_access_token(access_key, access_secret)

        api_access=tweepy.API(api)

        timeline=[]

        try:
                timeline=api_access.user_timeline(screen_name=handle, count=200, tweet_mode="extended")
        except tweepy.TweepError as error:
                print("An error has occurred: ", error)
                return
        filename=handle+'.txt'
        file=open(filename, 'w')
        for i in range (0, len(timeline)):
                file.write(timeline[i].full_text.encode('utf-8').strip())
                file.write('\n')
        file.close()

access_Twitter(sys.argv[1])
