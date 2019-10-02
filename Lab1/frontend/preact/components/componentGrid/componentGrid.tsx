import { h } from 'preact';
import { Card } from 'preact-fluid';

import Temp from '../temp/temp';
import Graph from '../graph/graph';
import Controls from '../controls/controls';

import style from './componentGrid.scss';

const ComponentGrid = () => (
    <div>
        <div class={style.Grid}>
            <Card className={`${style.TempCard} ${style.Card}`}>
                <Temp />
            </Card>
            <Card className={`${style.GraphCard} ${style.Card}`}>
                <Graph />
            </Card>
            <Card className={`${style.ControlsCard} ${style.Card}`}>
                <Controls />
            </Card>
        </div>
    </div>
);

export default ComponentGrid;
