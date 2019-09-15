import { h } from 'preact';
import { Card } from 'preact-fluid';

import Temp from '../temp/temp';
import Graph from '../graph/graph';
import Controls from '../controls/controls';

import style from './componentGrid.scss';

const ComponentGrid = () => (
    <div class={style.Grid}>
        <Card className={style.TempCard}>
            <Temp />
        </Card>
        <Card className={style.GraphCard}>
            <Graph />
        </Card>
        <Card className={style.ControlsCard}>
            <Controls />
        </Card>
    </div>
);

export default ComponentGrid;
