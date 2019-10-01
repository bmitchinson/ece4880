import { h, Component } from 'preact';
import { database } from '../firebase/firebase';
import { TrendChart, TimestampArray } from 'preact-charts';
import { CubeGrid } from 'styled-loaders';

import style from './graph.scss';

const tempRef = database.collection('temps');
const tempObj = tempRef.orderBy('time', 'desc').limit(300);

interface MyProps {}
interface MyState {
    loading: boolean;
    dataArray: TimestampArray;
    dataCoverArray: TimestampArray;
}

const LOWER_BOUND = 10;
const UPPER_BOUND = 50;

const tempQueue = [];

export default class Graph extends Component<MyProps, MyState> {
    constructor() {
        super();
        const dataArray = [];
        for (let i = 300; i > 0; i -= 1) {
            const time = new Date();
            time.setSeconds(time.getSeconds() - i);
            dataArray.push({
                timestamp: time,
                temp: -100
            });
        }
        const dataCoverArray = getCoverArray(dataArray);
        // Set loading state

        this.state = { dataArray, dataCoverArray, loading: true };
    }

    componentDidMount() {
        // Create 300 slots
        // [0] oldest
        const dataArray = this.state.dataArray;

        const zeroMinAgo = Math.floor(
            dataArray[299].timestamp.getTime() / 1000
        );
        const fiveMinAgo = Math.floor(dataArray[0].timestamp.getTime() / 1000);

        // Fetch last 300 from firebase
        let howmany = 0;
        tempObj
            .get()
            .then(snap => {
                snap.forEach(doc => {
                    howmany += 1;
                    // Newest in [0]
                    const seconds = doc.data().time.seconds;
                    const temp = doc.data().temp;
                    if (seconds >= fiveMinAgo && seconds <= zeroMinAgo) {
                        const pos = 299 - (zeroMinAgo - seconds);
                        dataArray[pos] = {
                            timestamp: new Date(seconds * 1000),
                            temp: temp.toFixed(1)
                        };
                    } else {
                        console.log('data not in range:', doc.data());
                    }
                });
                console.log('how many', howmany);
                // Set an onChange that then populates a queue of incoming temps
                // Every second, pull from that queue or create a dud if queue's empty
                setInterval(this.addPointFromQueue, 1000);
                this.setState({ loading: false });
            })
            .catch(e => console.log('Error getting documents:', e));

        // After set a function that every second:
        //   queries for the most recent one:
        //   if it's within a second of current time:
        //      push it to existing 300 slots
        //   Otherwise:
        //      add a -100 at the current time
        //   remove end.
        //
    }

    addPointFromQueue = () => {
        const dataArrayEdit = this.state.dataArray;
        dataArrayEdit.splice(0, 1);

        if (tempQueue.length) {
        } else {
            dataArrayEdit.push({
                timestamp: new Date(),
                temp: -100
            });
        }
        const dataCoverArrayEdit = getCoverArray(dataArrayEdit);

        this.setState({
            dataArray: dataArrayEdit,
            dataCoverArray: dataCoverArrayEdit
        });
    };

    addPointToQueue = () => {};

    // addNewPointToDataArray = () => {
    //     const dataArrayEdit = this.state.dataArray;
    //     const time = new Date();
    //     dataArrayEdit.push({
    //         timestamp: time,
    //         temp: Math.random() * 60 - 10
    //     });
    //     dataArrayEdit.sort((a, b) => (a.timestamp < b.timestamp ? -1 : 1));
    //     dataArrayEdit.splice(0, 1);
    //     const dataCoverArrayEdit = getCoverArray(dataArrayEdit);

    //     this.setState({
    //         dataArray: dataArrayEdit,
    //         dataCoverArray: dataCoverArrayEdit
    //     });
    // };

    render() {
        const { dataArray, dataCoverArray, loading } = this.state;

        return !loading ? (
            <div class={style.ContainGraph}>
                <div class={style.ContainChart}>
                    <TrendChart
                        lineColour="#90d7c2"
                        //lineColourTwo="#ffffff"
                        name="Temps"
                        x="timestamp"
                        y="temp"
                        data={getBoundedDataArray(dataArray)}
                        //dataSetTwo={getBoundedDataArray(dataCoverArray)}
                        margin={{ top: 20, right: 20, left: 40, bottom: 10 }}
                        axisControl={false}
                        tooltip={false}
                    />
                </div>
                <div class={style.XAxisItems}>
                    <div class={`${style.XAxisItem} ${style.ThreeHundred}`}>
                        300
                    </div>
                    <div class={`${style.XAxisItem} ${style.TwoTwentyFive}`}>
                        225
                    </div>
                    <div class={`${style.XAxisItem} ${style.OneFifty}`}>
                        150
                    </div>
                    <div class={`${style.XAxisItem} ${style.SevenFive}`}>
                        75
                    </div>
                    <div class={`${style.XAxisItem} ${style.Zero}`}>0</div>
                </div>
                <div class={style.XAxisLabels}>
                    <div class={style.XAxisLabel}>(Seconds Ago)</div>
                </div>
            </div>
        ) : (
            <CubeGrid color="#90d7c2" size="60px" />
        );
    }
}

const getBoundedDataArray = array => {
    return array
        .map(val => {
            if (val.temp > UPPER_BOUND) {
                return {
                    timestamp: new Date(val.timestamp),
                    temp: UPPER_BOUND
                };
            } else if (val.temp < LOWER_BOUND) {
                return {
                    timestamp: new Date(val.timestamp),
                    temp: LOWER_BOUND
                };
            } else return val;
        })
        .sort((a, b) => (a.timestamp < b.timestamp ? -1 : 1));
};

// const getFakeDataOne = () => {
//     const dataArray = [];
//     for (let i = 0; i < 100; i += 1) {
//         const time = new Date();
//         time.setSeconds(time.getSeconds() - i);
//         dataArray.push({
//             timestamp: time,
//             temp: Math.random() * 60 - 10
//         });
//     }
//     for (let i = 100; i < 200; i += 1) {
//         const time = new Date();
//         time.setSeconds(time.getSeconds() - i);
//         dataArray.push({
//             timestamp: time,
//             temp: -100
//         });
//     }
//     for (let i = 200; i < 300; i += 1) {
//         const time = new Date();
//         time.setSeconds(time.getSeconds() - i);
//         dataArray.push({
//             timestamp: time,
//             temp: Math.random() * 60 - 10
//         });
//     }
//     dataArray.sort((a, b) => (a.timestamp < b.timestamp ? -1 : 1));
//     return dataArray;
// };

const getCoverArray = dataArray => {
    const dataCoverArray = [];

    dataCoverArray.push({
        timestamp: dataArray[0].timestamp,
        temp: 0
    });
    dataCoverArray.push({
        timestamp: dataArray[dataArray.length - 1].timestamp,
        temp: 0
    });

    dataArray.forEach((ele, index, dataArray) => {
        const canLookBehind: boolean = index !== 0;
        const canLookAhead: boolean = index !== dataArray.length - 1;
        const isNeg100: boolean = ele.temp === -100;

        if (isNeg100 && canLookBehind && dataArray[index - 1].temp !== -100) {
            const pastTemp = dataArray[index - 1].temp;
            const pastTimestamp = dataArray[index - 1].timestamp;
            const shiftTimestamp = new Date(pastTimestamp);
            shiftTimestamp.setMilliseconds(pastTimestamp.getMilliseconds() - 2);

            dataCoverArray.push({
                timestamp: shiftTimestamp,
                temp: 0
            });
            dataCoverArray.push({
                timestamp: pastTimestamp,
                temp: pastTemp
            });
            dataCoverArray.push({
                timestamp: ele.timestamp,
                temp: 0
            });
        } else if (
            isNeg100 &&
            canLookAhead &&
            dataArray[index + 1].temp !== -100
        ) {
            const nextTemp = dataArray[index + 1].temp;
            const nextTimestamp = dataArray[index + 1].timestamp;
            const shiftTimestamp = new Date(nextTimestamp);
            shiftTimestamp.setMilliseconds(nextTimestamp.getMilliseconds() + 2);
            dataCoverArray.push({
                timestamp: ele.timestamp,
                temp: 0
            });
            dataCoverArray.push({
                timestamp: nextTimestamp,
                temp: nextTemp
            });
            dataCoverArray.push({
                timestamp: shiftTimestamp,
                temp: 0
            });
        }
    });
    // dataCoverArray.sort((a, b) => (a.timestamp < b.timestamp ? -1 : 1));
    return dataCoverArray;
};
