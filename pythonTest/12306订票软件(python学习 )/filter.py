import log
import json
import time


class Filter():
    def filter(self, start_time_str, train, config, loop=True):

        if train is None or len(train) == 0 or train[0] is None or len(train[0]) == 1:
            return

        log_ = log.Log('filter.log', 'a')
        #log_.write(json.dumps(train) + "\n" + '...................................................' + "\n")

        #预定席位类型
        favoriteSeats = config.getDictsSequence('defaultSetting', 'seatType')
        favoriteTrain = config.getDictsSequence('defaultSetting', 'favoriteTrain')
        minimumTicketNumber = int(config.getDictsSequence('defaultSetting', 'minimumTicketNumber'))
        autoTopSelectedTrain = config.getDictsSequence('defaultSetting', 'autoTopSelectedTrain')
        onlyDisplayFilterTrain = config.getDictsSequence('defaultSetting', 'onlyDisplayFilterTrain')

        seatTypeToQuery = config.get('seatTypeToQuery')
        seatTypeArr = config.get('seatTypeArr')

        #过滤车次
        if len(favoriteTrain) > 0 and len(favoriteTrain[0]) > 0:
            train = [i for i in train if i['station_train_code'] in favoriteTrain]

        ##过滤不能预定车票
        #if int(onlyDisplayFilterTrain) == 1:
        #    train = [i for i in train if i['buttonTextInfo'] == '预订']

        favoriteSeats = favoriteSeats if favoriteSeats and len(favoriteSeats) > 0 else seatTypeArr.keys()

        favoriteSeatsInQuery = [seatTypeToQuery[i] for i in favoriteSeats]

        validTrain = []
        validTrainId = []
        bestSeat = None

        #print('*******favoriteSeats********')
        #print(train)

        #过滤不能预定车票席位
        start_time_str_b = start_time_str.split('--')[0]
        start_time_str_e = start_time_str.split('--')[1]
        try:
            for i in range(len(favoriteSeats)):
                favoriteSeat = favoriteSeatsInQuery[i]

                for j in train:

                    start_time = int(
                        time.mktime(
                            time.strptime(time.strftime('%Y-%m-%d') + ' ' + str(j['start_time']), '%Y-%m-%d %H:%M')))

                    start_time__b = int(
                        time.mktime(
                            time.strptime(time.strftime('%Y-%m-%d') + ' ' + start_time_str_b, '%Y-%m-%d %H:%M')))

                    start_time_e = int(
                        time.mktime(
                            time.strptime(time.strftime('%Y-%m-%d') + ' ' + start_time_str_e, '%Y-%m-%d %H:%M')))
                    if start_time__b <= start_time and start_time_e >= start_time and j[
                        favoriteSeat] is not None and not j[favoriteSeat] in ('--', '无', '*') and (
                                str(j[favoriteSeat]) == '有' or int(j[favoriteSeat]) >= minimumTicketNumber):
                        validTrain.append(j)
                        validTrainId.append(j['station_train_code'])
                        if not bestSeat:
                            bestSeat = [key for key, value in seatTypeToQuery.items() if value == favoriteSeat][0]

        except Exception as e:
            pass
            #log_.write(e + "\n" + '####################################################' + "\n\n")

        train1 = [i for i in train if not i['station_train_code'] in validTrainId]

        if int(autoTopSelectedTrain) == 1:
            ##过滤不能预定车票
            if int(onlyDisplayFilterTrain) == 1:
                train = validTrain
            else:
                train = validTrain + train1

            return {'trains': train, 'filterTrain': validTrain[0] if len(validTrain) > 0 else [],
                    'seat': bestSeat}
        else:

            ##过滤不能预定车票
            if int(onlyDisplayFilterTrain) == 1:
                train = validTrain

            return {'trains': train, 'filterTrain': validTrain[0] if len(validTrain) > 0 else [], 'seat': bestSeat}


